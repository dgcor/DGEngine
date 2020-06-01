/*
 *  AudioStream.cpp
 *  sfeMovie project
 *
 *  Copyright (C) 2010-2015 Lucas Soltic
 *  lucas.soltic@orange.fr
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libavutil/samplefmt.h>
#include <libswresample/swresample.h>
}

#include "AudioStream.hpp"
#include <cstring>
#include <sfeMovie/Movie.hpp>

namespace sfe
{
	namespace
	{
		void waitForStatusUpdate(const sf::SoundStream& stream, sf::SoundStream::Status expectedStatus)
		{
			// Wait for status to update
			sf::Clock timeout;
			while (stream.getStatus() != expectedStatus &&
				timeout.getElapsedTime() < sf::seconds(5))
			{
				sf::sleep(sf::microseconds(10));
			}
		}

		constexpr int BytesPerSample = sizeof(sf::Int16); // Signed 16 bits audio sample
	}

	AudioStream::AudioStream(AVFormatContext*& formatCtx, AVStream*& stream,
		DataSource& dataSource, Timer* timer) :
		Stream(formatCtx, stream, dataSource, timer)
	{
		if (Stream::hasError() == true)
		{
			return;
		}

		m_audioFrame = av_frame_alloc();
		if (m_audioFrame == nullptr)
		{
			return;
		}

		// Get some audio informations
		m_sampleRatePerChannel = m_stream->codecpar->sample_rate;

		// Alloc a two seconds buffer
		m_samplesBuffer = (sf::Int16*)av_malloc(sizeof(sf::Int16) * av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO)
			* m_sampleRatePerChannel * 2); // * 2 is for 2 seconds
		if (m_samplesBuffer == nullptr)
		{
			return;
		}

		// Initialize the sf::SoundStream
		// Whatever the channel count is, it'll we resampled to stereo
		sf::SoundStream::initialize(av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO), m_sampleRatePerChannel);

		// Initialize resampler to be able to give signed 16 bits samples to SFML
		initResampler();
	}

	/** Default destructor
	 */
	AudioStream::~AudioStream()
	{
		if (m_audioFrame != nullptr)
		{
			av_frame_free(&m_audioFrame);
		}
		if (m_samplesBuffer != nullptr)
		{
			av_free(m_samplesBuffer);
		}
		if (m_dstData != nullptr)
		{
			av_freep(&m_dstData[0]);
		}
		av_freep(&m_dstData);
		swr_free(&m_swrCtx);
	}

	bool AudioStream::hasError() const noexcept
	{
		if (Stream::hasError() == true ||
			m_audioFrame == nullptr ||
			m_samplesBuffer == nullptr ||
			m_swrCtx == nullptr)
		{
			return true;
		}
		return false;
	}

	void AudioStream::flushBuffers()
	{
		auto sfStatus = sf::SoundStream::getStatus();
		if (sfStatus == sf::SoundStream::Playing)
		{
			return;
		}
		// Flush audio driver/OpenAL/SFML buffer
		if (sfStatus != sf::SoundStream::Stopped)
		{
			sf::SoundStream::stop();
		}
		m_extraAudioTime = sf::Time::Zero;
		Stream::flushBuffers();
	}

	MediaType AudioStream::getStreamKind() const
	{
		return Audio;
	}

	void AudioStream::update()
	{
		switch (sf::SoundStream::getStatus())
		{
		case sf::SoundStream::Playing:
			setStatus(sfe::Playing);
			break;
		case sf::SoundStream::Paused:
			setStatus(sfe::Paused);
			break;
		case sf::SoundStream::Stopped:
			setStatus(sfe::Stopped);
			break;
		default:
			break;
		}
	}

	bool AudioStream::fastForward(sf::Time targetPosition)
	{
		sf::Time currentPosition;
		sf::Time pktDuration;

		do
		{
			if (computeEncodedPosition(currentPosition) == false)
			{
				return false;
			}

			AVPacket* packet = popEncodedData();

			if (packet == nullptr)
			{
				return false;
			}

			pktDuration = packetDuration(packet);

			if (currentPosition > targetPosition)
			{
				// Computations with packet duration and stream position are not always very accurate so
				// this can happen some times. In such cases, the different is very small (less than 1ms)
				// so we just accept it

				m_extraAudioTime = sf::Time::Zero;

				// Reinsert, we don't want to decode now
				prependEncodedData(packet);
			}
			else if (currentPosition + pktDuration > targetPosition)
			{
				// Reinsert, we don't want to decode now
				prependEncodedData(packet);
				m_extraAudioTime = targetPosition - currentPosition;
			}
			else
			{
				av_packet_unref(packet);
				av_free(packet);
			}
		} while (currentPosition + pktDuration <= targetPosition);

		return true;
	}

	bool AudioStream::onGetData(sf::SoundStream::Chunk& data)
	{
		AVPacket* packet = nullptr;
		data.samples = m_samplesBuffer;

		const int stereoChannelCount = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);

		while (data.sampleCount < stereoChannelCount * m_sampleRatePerChannel &&
			(nullptr != (packet = popEncodedData())))
		{
			bool needsMoreDecoding = false;
			bool gotFrame = false;

			do
			{
				needsMoreDecoding = decodePacket(packet, m_audioFrame, gotFrame);

				if (gotFrame)
				{
					uint8_t* samplesBuffer = nullptr;
					int samplesCount = 0;

					resampleFrame(m_audioFrame, samplesBuffer, samplesCount);

					if (samplesBuffer == nullptr ||
						samplesCount <= 0)
					{
						break;
					}

					if (m_extraAudioTime > sf::Time::Zero)
					{
						int samplesToDiscard = timeToSamples(m_extraAudioTime);
						if (samplesToDiscard > samplesCount)
						{
							samplesToDiscard = samplesCount;
						}

						if (samplesToDiscard < stereoChannelCount && samplesCount > 0)
						{
							m_extraAudioTime = sf::Time::Zero;
						}
						else
						{
							samplesBuffer += samplesToDiscard * BytesPerSample;
							samplesCount -= samplesToDiscard;

							m_extraAudioTime -= samplesToTime(samplesToDiscard);
						}
					}

					std::memcpy((void*)(data.samples + data.sampleCount),
						samplesBuffer, samplesCount * BytesPerSample);
					data.sampleCount += samplesCount;
				}
			} while (needsMoreDecoding);

			bool leaveLoop = false;
			if (packet->data == nullptr)
			{
				leaveLoop = true;
			}

			av_packet_unref(packet);
			av_free(packet);

			if (leaveLoop == true)
			{
				break;
			}
		}
		return (packet != nullptr);
	}

	void AudioStream::onSeek(sf::Time timeOffset)
	{
		// not implemented
	}

	bool AudioStream::decodePacket(AVPacket* packet, AVFrame* outputFrame, bool& gotFrame)
	{
		int ret;
		gotFrame = false;

		ret = avcodec_send_packet(m_codecCtx, packet);
		if (ret < 0)
		{
			return false;
		}

		ret = avcodec_receive_frame(m_codecCtx, outputFrame);
		if (ret < 0)
		{
			if (ret == AVERROR(EAGAIN))
			{
				return true;
			}
			return false;
		}
		gotFrame = true;
		return false;
	}

	void AudioStream::initResampler()
	{
		if (m_swrCtx != nullptr)
		{
			return;
		}

		/* create resampler context */
		m_swrCtx = swr_alloc();
		if (m_swrCtx == nullptr)
		{
			return;
		}

		// Some media files don't define the channel layout, in this case take a default one
		// according to the channels' count
		if (m_stream->codecpar->channel_layout == 0)
		{
			m_stream->codecpar->channel_layout = av_get_default_channel_layout(m_stream->codecpar->channels);
		}

		/* set options */
		av_opt_set_int(m_swrCtx, "in_channel_layout", m_stream->codecpar->channel_layout, 0);
		av_opt_set_int(m_swrCtx, "in_sample_rate", m_stream->codecpar->sample_rate, 0);
		av_opt_set_sample_fmt(m_swrCtx, "in_sample_fmt", (AVSampleFormat)m_stream->codecpar->format, 0);
		av_opt_set_int(m_swrCtx, "out_channel_layout", AV_CH_LAYOUT_STEREO, 0);
		av_opt_set_int(m_swrCtx, "out_sample_rate", m_stream->codecpar->sample_rate, 0);
		av_opt_set_sample_fmt(m_swrCtx, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);

		/* initialize the resampling context */
		auto err = swr_init(m_swrCtx);
		if (err < 0)
		{
			return;
		}

		/* compute the number of converted samples: buffering is avoided
		 * ensuring that the output buffer will contain at least all the
		 * converted input samples */
		m_maxDstNbSamples = m_dstNbSamples = 1024;

		/* Create the resampling output buffer */
		m_dstNbChannels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
		err = av_samples_alloc_array_and_samples(&m_dstData, &m_dstLinesize, m_dstNbChannels,
			m_dstNbSamples, AV_SAMPLE_FMT_S16, 0);
		if (err < 0)
		{
			return;
		}
	}

	void AudioStream::resampleFrame(const AVFrame* frame, uint8_t*& outSamples, int& outNbSamples)
	{
		if (m_swrCtx == nullptr ||
			frame == nullptr)
		{
			return;
		}

		int src_rate, dst_rate, err, dst_bufsize;
		src_rate = dst_rate = frame->sample_rate;

		/* compute destination number of samples */
		m_dstNbSamples = (int)av_rescale_rnd(swr_get_delay(m_swrCtx, src_rate) +
			frame->nb_samples, dst_rate, src_rate, AV_ROUND_UP);
		if (m_dstNbSamples > m_maxDstNbSamples)
		{
			av_free(m_dstData[0]);
			err = av_samples_alloc(m_dstData, &m_dstLinesize, m_dstNbChannels,
				m_dstNbSamples, AV_SAMPLE_FMT_S16, 1);
			if (err < 0)
			{
				return;
			}
			m_maxDstNbSamples = m_dstNbSamples;
		}

		/* convert to destination format */
		err = swr_convert(m_swrCtx, m_dstData, m_dstNbSamples, (const uint8_t**)frame->extended_data, frame->nb_samples);
		if (err < 0)
		{
			return;
		}

		dst_bufsize = av_samples_get_buffer_size(&m_dstLinesize, m_dstNbChannels,
			err, AV_SAMPLE_FMT_S16, 1);
		if (dst_bufsize < 0)
		{
			return;
		}

		outNbSamples = dst_bufsize / av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
		outSamples = m_dstData[0];
	}

	int AudioStream::timeToSamples(const sf::Time& time) const
	{
		const int channelCount = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
		int64_t samplesPerSecond = (int64_t)m_sampleRatePerChannel * channelCount;
		int64_t samples = (samplesPerSecond * time.asMicroseconds()) / 1000000;
		// We don't want SFML to be confused by interverting left and right speaker sound in case
		// samples are interleaved
		if (samples % channelCount != 0)
		{
			samples -= samples % channelCount;
		}
		return (int)samples;
	}

	sf::Time AudioStream::samplesToTime(int nbSamples) const
	{
		int64_t samplesPerChannel = nbSamples / av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
		int64_t microseconds = 1000000 * samplesPerChannel / (int)m_sampleRatePerChannel;
		return sf::microseconds(microseconds);
	}

	void AudioStream::willPlay(const Timer& timer)
	{
		Stream::willPlay(timer);

		if (Stream::getStatus() == sfe::Stopped)
		{
			sf::Time initialTime = sf::SoundStream::getPlayingOffset();
			sf::Clock timeout;
			sf::SoundStream::play();

			// Some audio drivers take time before the sound is actually played
			// To avoid desynchronization with the timer, we don't return
			// until the audio stream is actually started
			while (sf::SoundStream::getPlayingOffset() == initialTime &&
				timeout.getElapsedTime() < sf::seconds(5))
			{
				sf::sleep(sf::microseconds(10));
			}
		}
		else
		{
			sf::SoundStream::play();
			waitForStatusUpdate(*this, sf::SoundStream::Playing);
		}
	}

	void AudioStream::didPlay(const Timer& timer, sfe::Status previousStatus)
	{
		if (SoundStream::getStatus() == SoundStream::Playing)
		{
			Stream::didPlay(timer, previousStatus);
		}
	}

	void AudioStream::didPause(const Timer& timer, sfe::Status previousStatus)
	{
		if (sf::SoundStream::getStatus() == sf::SoundStream::Playing)
		{
			sf::SoundStream::pause();
			waitForStatusUpdate(*this, sf::SoundStream::Paused);
		}
		Stream::didPause(timer, previousStatus);
	}

	void AudioStream::didStop(const Timer& timer, sfe::Status previousStatus)
	{
		sf::SoundStream::stop();
		waitForStatusUpdate(*this, sf::SoundStream::Stopped);
		Stream::didStop(timer, previousStatus);
	}
}
