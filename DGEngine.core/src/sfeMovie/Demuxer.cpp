/*
 *  Demuxer.cpp
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
#include <libswscale/swscale.h>
#include <stdint.h>
}

#include "Demuxer.hpp"
#include "AudioStream.hpp"
#include <mutex>
#include "TimerPriorities.hpp"
#include "VideoStream.hpp"

namespace sfe
{
	Demuxer::Demuxer(const std::string_view sourceFile, Timer* timer,
		VideoStream::Delegate& videoDelegate) : m_timer(timer)
	{
		if (sourceFile.empty() == true ||
			timer == nullptr)
		{
			return;
		}
		init(sourceFile.data(), videoDelegate);
	}

	Demuxer::Demuxer(sf::InputStream& inputStream, Timer* timer,
		VideoStream::Delegate& videoDelegate) : m_timer(timer)
	{
		if (timer == nullptr)
		{
			return;
		}

		m_streamContext = std::make_unique<InputStreamIOContext>(&inputStream);
		m_formatCtx = ::avformat_alloc_context();
		if (m_formatCtx == nullptr)
		{
			return;
		}
		m_formatCtx->pb = m_streamContext->getAVIOContext();
		init("", videoDelegate);
	}

	void Demuxer::init(const char* fileName, VideoStream::Delegate& videoDelegate)
	{
		int err = 0;

		// Open the movie file
		err = avformat_open_input(&m_formatCtx, fileName, nullptr, nullptr);
		if (err != 0 ||
			m_formatCtx == nullptr)
		{
			return;
		}

		// Read the general movie informations
		err = avformat_find_stream_info(m_formatCtx, nullptr);
		if (err < 0)
		{
			return;
		}

		// Get the media duration if possible (otherwise rely on the streams)
		if (m_formatCtx->duration != AV_NOPTS_VALUE)
		{
			int64_t secs, us;
			secs = m_formatCtx->duration / AV_TIME_BASE;
			us = m_formatCtx->duration % AV_TIME_BASE;
			m_duration = sf::seconds(secs + (float)us / AV_TIME_BASE);
		}

		// Find all interesting streams
		for (unsigned int i = 0; i < m_formatCtx->nb_streams; i++)
		{
			AVStream* & ffstream = m_formatCtx->streams[i];

			switch (ffstream->codecpar->codec_type)
			{
			case AVMEDIA_TYPE_VIDEO:
			{
				auto stream = std::make_unique<VideoStream>(m_formatCtx, ffstream, *this, m_timer, videoDelegate);
				if (stream->hasError() == true)
				{
					continue;
				}
				if (m_duration == sf::Time::Zero)
				{
					extractDurationFromStream(ffstream);
				}
				if (stream != nullptr)
				{
					m_videoStreams.push_back(std::make_pair(std::move(stream), ffstream->index));
				}
				break;
			}
			case AVMEDIA_TYPE_AUDIO:
			{
				auto stream = std::make_unique<AudioStream>(m_formatCtx, ffstream, *this, m_timer);
				if (stream->hasError() == true)
				{
					continue;
				}
				if (m_duration == sf::Time::Zero)
				{
					extractDurationFromStream(ffstream);
				}
				if (stream != nullptr)
				{
					m_audioStreams.push_back(std::make_pair(std::move(stream), ffstream->index));
				}
				break;
			}
			default:
				break;
			}
		}
		m_timer->addObserver(*this, DemuxerTimerPriority);
	}

	Demuxer::~Demuxer()
	{
		if (m_timer->getStatus() != Stopped)
		{
			m_timer->stop();
		}

		m_timer->removeObserver(*this);

		// NB: these manual cleaning are important for the AVFormatContext to be deleted last, otherwise
		// the streams lose their connection to the codec and leak
		m_audioStreams.clear();
		m_videoStreams.clear();
		m_connectedStreams[AudioStreamIndex] = nullptr;
		m_connectedStreams[VideoStreamIndex] = nullptr;

		if (m_formatCtx != nullptr)
		{
			// Be very careful with this call: it'll also destroy its codec contexts and streams
			avformat_close_input(&m_formatCtx);
		}
		flushBuffers();
	}

	const std::vector<std::pair<std::unique_ptr<AudioStream>, int>>& Demuxer::getAudioStreams() const
	{
		return m_audioStreams;
	}

	const std::vector<std::pair<std::unique_ptr<VideoStream>, int>>& Demuxer::getVideoStreams() const
	{
		return m_videoStreams;
	}

	std::vector<StreamDescriptor> Demuxer::computeStreamDescriptors(MediaType type) const
	{
		std::vector<StreamDescriptor> entries;
		for (const auto& pair : m_audioStreams)
		{
			if (pair.first->getStreamKind() == type)
			{
				StreamDescriptor entry;
				entry.type = type;
				entry.identifier = pair.second;
				entry.language = pair.first->getLanguage();
				entries.push_back(entry);
			}
		}
		for (const auto& pair : m_videoStreams)
		{
			if (pair.first->getStreamKind() == type)
			{
				StreamDescriptor entry;
				entry.type = type;
				entry.identifier = pair.second;
				entry.language = pair.first->getLanguage();
				entries.push_back(entry);
			}
		}
		return entries;
	}

	void Demuxer::selectAudioStream(AudioStream* stream)
	{
		auto oldStatus = m_timer->getStatus();
		if (oldStatus != Stopped)
		{
			return;
		}
		if (oldStatus == Playing)
		{
			m_timer->pause();
		}
		if (stream != m_connectedStreams[AudioStreamIndex])
		{
			if (m_connectedStreams[AudioStreamIndex] != nullptr)
			{
				m_connectedStreams[AudioStreamIndex]->disconnect();
			}
			if (stream != nullptr)
			{
				stream->connect();
			}
			m_connectedStreams[AudioStreamIndex] = stream;
			m_pendingDataForActiveStreams[AudioStreamIndex].first = stream;
			flushBuffer(m_pendingDataForActiveStreams[AudioStreamIndex].second);
		}
		if (oldStatus == Playing)
		{
			m_timer->play();
		}
	}

	void Demuxer::selectFirstAudioStream()
	{
		if (m_audioStreams.empty() == false)
		{
			selectAudioStream(m_audioStreams.begin()->first.get());
		}
	}

	AudioStream* Demuxer::getSelectedAudioStream() const
	{
		return (AudioStream*)m_connectedStreams[AudioStreamIndex];
	}

	void Demuxer::selectVideoStream(VideoStream* stream)
	{
		Status oldStatus = m_timer->getStatus();
		if (oldStatus != Stopped)
		{
			return;
		}
		if (oldStatus == Playing)
		{
			m_timer->pause();
		}
		if (stream != m_connectedStreams[VideoStreamIndex])
		{
			if (m_connectedStreams[VideoStreamIndex])
			{
				m_connectedStreams[VideoStreamIndex]->disconnect();
			}
			if (stream != nullptr)
			{
				stream->connect();
			}
			m_connectedStreams[VideoStreamIndex] = stream;
			m_pendingDataForActiveStreams[VideoStreamIndex].first = stream;
			flushBuffer(m_pendingDataForActiveStreams[VideoStreamIndex].second);
		}
		if (oldStatus == Playing)
		{
			m_timer->play();
		}
	}

	void Demuxer::selectFirstVideoStream()
	{
		if (m_videoStreams.empty() == false)
		{
			selectVideoStream(m_videoStreams.begin()->first.get());
		}
	}

	VideoStream* Demuxer::getSelectedVideoStream() const
	{
		return (VideoStream*)m_connectedStreams[VideoStreamIndex];
	}

	void Demuxer::feedStream(Stream& stream)
	{
		if (stream.isPassive() == true)
		{
			return;
		}

		sf::Lock l(m_synchronized);

		while ((didReachEndOfFile() == false || hasPendingDataForStream(stream) == true)
			&& stream.needsMoreData() == true)
		{
			AVPacket* pkt = nullptr;

			pkt = gatherQueuedPacketForStream(stream);

			if (pkt == nullptr)
			{
				pkt = readPacket();
			}
			if (pkt == nullptr)
			{
				m_eofReached = true;
			}
			else
			{
				if (distributePacket(pkt, stream) == false)
				{
					av_packet_unref(pkt);
					av_free(pkt);
				}
			}
		}
	}

	void Demuxer::update()
	{
		for (const auto& pair : m_audioStreams)
		{
			pair.first->update();
		}
		for (const auto& pair : m_videoStreams)
		{
			pair.first->update();
		}
	}

	bool Demuxer::didReachEndOfFile() const
	{
		return m_eofReached;
	}

	sf::Time Demuxer::getDuration() const
	{
		return m_duration;
	}

	AVPacket* Demuxer::readPacket()
	{
		sf::Lock l(m_synchronized);

		AVPacket* pkt = av_packet_alloc();
		if (pkt == nullptr)
		{
			return nullptr;
		}

		if (av_read_frame(m_formatCtx, pkt) < 0)
		{
			av_packet_unref(pkt);
			av_free(pkt);
			pkt = nullptr;
		}

		return pkt;
	}

	void Demuxer::flushBuffers()
	{
		sf::Lock l(m_synchronized);

		for (auto& pair : m_pendingDataForActiveStreams)
		{
			pair.first = nullptr;
			flushBuffer(pair.second);
		}
	}

	void Demuxer::flushBuffer(std::list<AVPacket*>& buffer)
	{
		for (auto packet : buffer)
		{
			av_packet_unref(packet);
			av_free(packet);
		}
		buffer.clear();
	}

	void Demuxer::queueEncodedData(AVPacket* packet)
	{
		sf::Lock l(m_synchronized);

		for (auto stream : m_connectedStreams)
		{
			if (stream->canUsePacket(packet))
			{
				for (auto& obj : m_pendingDataForActiveStreams)
				{
					if (obj.first == stream)
					{
						obj.second.push_back(packet);
						return;
					}
				}
			}
		}

		av_packet_unref(packet);
		av_free(packet);
	}

	bool Demuxer::hasPendingDataForStream(const Stream& stream) const
	{
		sf::Lock l(m_synchronized);

		for (auto& obj : m_pendingDataForActiveStreams)
		{
			if (obj.first == &stream)
			{
				return obj.second.empty() == false;
			}
		}
		return false;
	}

	AVPacket* Demuxer::gatherQueuedPacketForStream(Stream& stream)
	{
		sf::Lock l(m_synchronized);

		for (auto& obj : m_pendingDataForActiveStreams)
		{
			if (obj.first == &stream)
			{
				auto& pendingPackets = obj.second;
				if (pendingPackets.empty() == false)
				{
					AVPacket* packet = pendingPackets.front();
					pendingPackets.pop_front();
					return packet;
				}
			}
		}
		return nullptr;
	}

	bool Demuxer::distributePacket(AVPacket* packet, Stream& stream)
	{
		sf::Lock l(m_synchronized);
		if (packet == nullptr)
		{
			return false;
		}
		bool distributed = false;
		for (const auto& s : m_audioStreams)
		{
			if (s.second != packet->stream_index)
			{
				continue;
			}
			const auto targetStream = s.first.get();

			// We don't want to store the packets for inactive streams,
			// let them be freed
			if (targetStream == getSelectedAudioStream())
			{
				if (targetStream == &stream || targetStream->isPassive() == true)
				{
					targetStream->pushEncodedData(packet);
				}
				else
				{
					queueEncodedData(packet);
				}
				distributed = true;
			}
		}
		for (const auto& s : m_videoStreams)
		{
			if (s.second != packet->stream_index)
			{
				continue;
			}
			const auto targetStream = s.first.get();

			// We don't want to store the packets for inactive streams,
			// let them be freed
			if (targetStream == getSelectedVideoStream())
			{
				if (targetStream == &stream || targetStream->isPassive() == true)
				{
					targetStream->pushEncodedData(packet);
				}
				else
				{
					queueEncodedData(packet);
				}
				distributed = true;
			}
		}
		return distributed;
	}

	void Demuxer::extractDurationFromStream(const AVStream* stream)
	{
		if (m_duration != sf::Time::Zero)
		{
			return;
		}
		if (stream->duration != AV_NOPTS_VALUE)
		{
			int64_t secs, us;
			secs = stream->duration / AV_TIME_BASE;
			us = stream->duration % AV_TIME_BASE;
			m_duration = sf::seconds(secs + (float)us / AV_TIME_BASE);
		}
	}

	void Demuxer::requestMoreData(Stream& starvingStream)
	{
		if (starvingStream.isPassive() == true)
		{
			return;
		}
		sf::Lock l(m_synchronized);
		feedStream(starvingStream);
	}

	void Demuxer::resetEndOfFileStatus()
	{
		m_eofReached = false;
	}

	bool Demuxer::didSeek(const Timer &timer, sf::Time oldPosition)
	{
		resetEndOfFileStatus();
		sf::Time newPosition = timer.getOffset();

		if (m_connectedStreams[AudioStreamIndex] == nullptr &&
			m_connectedStreams[VideoStreamIndex] == nullptr)
		{
			return false;
		}
		// Trivial seeking to beginning
		if (newPosition == sf::Time::Zero)
		{
			int64_t timestamp = 0;

			if (m_formatCtx->iformat->flags & AVFMT_SEEK_TO_PTS && m_formatCtx->start_time != AV_NOPTS_VALUE)
			{
				timestamp += m_formatCtx->start_time;
			}

			// Flush all streams
			for (auto stream : m_connectedStreams)
			{
				if (stream != nullptr)
				{
					stream->flushBuffers();
				}
			}
			flushBuffers();

			// Seek to beginning
			int err = avformat_seek_file(m_formatCtx, -1, INT64_MIN, timestamp, INT64_MAX, AVSEEK_FLAG_BACKWARD);
			if (err < 0)
			{
				return false;
			}
		}
		else // Seeking to some other position
		{
			// Initial target seek point
			int64_t timestamp = ((int64_t)newPosition.asSeconds()) * AV_TIME_BASE;

			// < 0 = before seek point
			// > 0 = after seek point
			std::vector<sf::Time> seekingGaps;

			static const float brokenSeekingThreshold = 60.f; // seconds
			int tooEarlyCount = 0;
			int tooLateCount = 0;
			int brokenSeekingCount = 0;
			int ffmpegSeekFlags = AVSEEK_FLAG_BACKWARD;

			do
			{
				// Flush all streams
				for (auto stream : m_connectedStreams)
				{
					if (stream != nullptr)
					{
						stream->flushBuffers();
					}
				}
				flushBuffers();

				// Seek to new estimated target
				if (m_formatCtx->iformat->flags & AVFMT_SEEK_TO_PTS &&
					m_formatCtx->start_time != AV_NOPTS_VALUE)
				{
					timestamp += m_formatCtx->start_time;
				}

				int err = avformat_seek_file(m_formatCtx, -1, timestamp - 10 * AV_TIME_BASE,
					timestamp, timestamp, ffmpegSeekFlags);
				if (err < 0)
				{
					return false;
				}

				// Compute the new gap
				for (auto stream : m_connectedStreams)
				{
					if (stream == nullptr ||
						stream->isPassive() == true)
					{
						continue;
					}
					sf::Time position;
					if (stream->computeEncodedPosition(position) == true)
					{
						seekingGaps.push_back(position - newPosition);
					}
				}

				tooEarlyCount = 0;
				tooLateCount = 0;
				brokenSeekingCount = 0;

				// Check the current situation
				for (const auto& gap : seekingGaps)
				{
					// < 0 = before seek point
					// > 0 = after seek point
					float absoluteDiff = fabs(gap.asSeconds());

					// Before seek point
					if (gap < sf::Time::Zero)
					{
						if (absoluteDiff > brokenSeekingThreshold)
						{
							brokenSeekingCount++;
							tooEarlyCount++;
						}
						// else: a bit early but not too much, this is the final situation we want
					}
					// After seek point
					else if (gap > sf::Time::Zero)
					{
						tooLateCount++;
						if (absoluteDiff > brokenSeekingThreshold)
						{
							brokenSeekingCount++;
						}
					}
				}

				// Define what to do next
				if (tooEarlyCount)
				{
					// Go forward by 1 sec
					timestamp += AV_TIME_BASE;
				}
				else if (tooLateCount)
				{
					// Go backward by 1 sec
					timestamp -= AV_TIME_BASE;
				}

				if (brokenSeekingCount)
				{
					if (ffmpegSeekFlags & AVSEEK_FLAG_ANY)
					{
						return false;
					}
					else
					{
						// Try to seek to non-key frame before giving up
						// Image may be wrong but it's better than nothing :)
						ffmpegSeekFlags |= AVSEEK_FLAG_ANY;
					}
				}
			} while (tooEarlyCount != 0 || tooLateCount != 0);
		}
		return true;
	}
}
