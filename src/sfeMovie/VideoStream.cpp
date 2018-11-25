/*
 *  VideoStream.cpp
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
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

#include "VideoStream.hpp"

namespace sfe
{
	VideoStream::VideoStream(AVFormatContext*& formatCtx, AVStream*& stream,
		DataSource& dataSource, Timer* timer, Delegate& delegate) :
		Stream(formatCtx, stream, dataSource, timer), m_delegate(delegate)
	{
		if (Stream::hasError() == true)
		{
			return;
		}

		m_rawVideoFrame = av_frame_alloc();
		if (m_rawVideoFrame == nullptr)
		{
			return;
		}

		// RGBA video buffer
#if LIBAVFORMAT_VERSION_MAJOR > 56
		auto avErr = av_image_alloc(m_rgbaVideoBuffer, m_rgbaVideoLinesize,
			m_stream->codecpar->width, m_stream->codecpar->height,
			AV_PIX_FMT_RGBA, 1);
#else
		auto avErr = av_image_alloc(m_rgbaVideoBuffer, m_rgbaVideoLinesize,
			m_stream->codec->width, m_stream->codec->height,
			AV_PIX_FMT_RGBA, 1);
#endif
		if (avErr < 0)
		{
			return;
		}

		// SFML video frame
#if LIBAVFORMAT_VERSION_MAJOR > 56
		auto texErr = m_texture.create(m_stream->codecpar->width, m_stream->codecpar->height);
#else
		auto texErr = m_texture.create(m_stream->codec->width, m_stream->codec->height);
#endif
		if (texErr == false)
		{
			return;
		}

		initRescaler();
	}

	VideoStream::~VideoStream()
	{
		if (m_rawVideoFrame != nullptr)
		{
			av_frame_free(&m_rawVideoFrame);
		}
		if (m_rgbaVideoBuffer[0] != nullptr)
		{
			av_freep(&m_rgbaVideoBuffer[0]);
		}
		if (m_swsCtx != nullptr)
		{
			sws_freeContext(m_swsCtx);
		}
	}

	bool VideoStream::hasError() const noexcept
	{
		if (Stream::hasError() == true ||
			m_texture.getNativeHandle() == 0 ||
			m_rawVideoFrame == nullptr ||
			m_rgbaVideoBuffer[0] == nullptr ||
			m_swsCtx == nullptr)
		{
			return true;
		}
		return false;
	}

	MediaType VideoStream::getStreamKind() const
	{
		return Video;
	}

	sf::Vector2i VideoStream::getFrameSize() const
	{
#if LIBAVFORMAT_VERSION_MAJOR > 56
		return sf::Vector2i(m_stream->codecpar->width, m_stream->codecpar->height);
#else
		return sf::Vector2i(m_stream->codec->width, m_stream->codec->height);
#endif
	}

	float VideoStream::getFrameRate() const
	{
		return static_cast<float>(av_q2d(av_guess_frame_rate(m_formatCtx, m_stream, nullptr)));
	}

	sf::Texture& VideoStream::getVideoTexture()
	{
		return m_texture;
	}

	void VideoStream::update()
	{
		sf::Time gap;
		bool couldComputeGap = false;
		while (getStatus() == Playing &&
			(couldComputeGap = getSynchronizationGap(gap)) &&
			gap < sf::Time::Zero)
		{
			if (onGetData(m_texture) == false)
			{
				setStatus(Stopped);
			}
			else
			{
				static const sf::Time skipFrameThreshold(sf::milliseconds(50));
				if (getSynchronizationGap(gap) && gap + skipFrameThreshold >= sf::Time::Zero)
				{
					m_delegate.didUpdateVideo(*this, m_texture);
				}
			}
		}
		if (!couldComputeGap)
		{
			setStatus(Stopped);
		}
	}

	void VideoStream::flushBuffers()
	{
		m_codecBufferingDelays.clear();
		Stream::flushBuffers();
	}

	bool VideoStream::fastForward(sf::Time targetPosition)
	{
		sf::Time position;
		bool couldGetPosition = false;
		while ((couldGetPosition = computeEncodedPosition(position)) &&
			position < targetPosition)
		{
			// We HAVE to decode the frames to get a full image when we reach the target position
			if (onGetData(m_texture) == false)
			{
				return false;
			}
		}
		return true;
	}

	void VideoStream::preload()
	{
		onGetData(m_texture);
	}

	bool VideoStream::onGetData(sf::Texture& texture)
	{
		AVPacket* packet = popEncodedData();
		bool gotFrame = false;
		bool goOn = false;

		if (packet != nullptr)
		{
			goOn = true;

			while (gotFrame == false &&
				packet != nullptr &&
				goOn == true)
			{
				bool needsMoreDecoding = false;

				goOn = decodePacket(packet, m_rawVideoFrame, gotFrame, needsMoreDecoding);

				if (gotFrame)
				{
					rescale(m_rawVideoFrame, m_rgbaVideoBuffer, m_rgbaVideoLinesize);
					texture.update(m_rgbaVideoBuffer[0]);
				}

				if (!gotFrame && goOn)
				{
					// Decoding went fine but did not produce an image. This means the decoder is working in
					// a pipelined way and wants more packets to output a full image. When the first full image will
					// be generated, the encoded data queue head pts will be late compared to the generated image pts
					// To take that into account we accumulate this time difference for reuse in getSynchronizationGap()
					m_codecBufferingDelays.push_back(packetDuration(packet));

#if LIBAVFORMAT_VERSION_MAJOR > 56
					if (m_codecBufferingDelays.size() > (size_t)m_stream->codecpar->video_delay)
					{
						m_codecBufferingDelays.pop_front();
					}
#else
					if (m_codecBufferingDelays.size() > (size_t)m_stream->codec->delay)
					{
						m_codecBufferingDelays.pop_front();
					}
#endif
				}

				if (needsMoreDecoding)
				{
					prependEncodedData(packet);
				}
				else
				{
#if LIBAVCODEC_VERSION_MAJOR > 56
					av_packet_unref(packet);
#else
					av_free_packet(packet);
#endif
					av_free(packet);
				}

				if (!gotFrame && goOn)
				{
					packet = popEncodedData();
				}
			}
		}
		return goOn;
	}

	bool VideoStream::getSynchronizationGap(sf::Time& gap)
	{
		sf::Time position;
		if (computeEncodedPosition(position) == true)
		{
			gap = (position - codecBufferingDelay()) - m_timer->getOffset();
			return true;
		}
		else
		{
			return false;
		}
	}

	bool VideoStream::decodePacket(AVPacket* packet, AVFrame* outputFrame, bool& gotFrame, bool& needsMoreDecoding)
	{
#if LIBAVCODEC_VERSION_MAJOR > 56
		int ret;
		gotFrame = false;
		needsMoreDecoding = false;

		ret = avcodec_send_packet(m_codecCtx, packet);
		if (ret < 0)
		{
			return false;
		}

		ret = avcodec_receive_frame(m_codecCtx, outputFrame);
		if (ret < 0)
		{
			needsMoreDecoding = (ret == AVERROR(EAGAIN));
			if (needsMoreDecoding == true)
			{
				return true;
			}
			return false;
		}
		gotFrame = true;
		return true;
#else
		int gotPicture = 0;
		needsMoreDecoding = false;
		int decodedLength = avcodec_decode_video2(m_stream->codec, outputFrame, &gotPicture, packet);

		gotFrame = (gotPicture != 0);

		if (decodedLength > 0 || gotFrame)
		{
			if (decodedLength < packet->size)
			{
				needsMoreDecoding = true;
				packet->data += decodedLength;
				packet->size -= decodedLength;
			}

			return true;
		}
		else
		{
			return false;
		}
#endif
	}

	void VideoStream::initRescaler()
	{
		/* create scaling context */
		int algorithm = SWS_FAST_BILINEAR;

		if (getFrameSize().x % 8 != 0 && getFrameSize().x * getFrameSize().y < 500000)
		{
			algorithm |= SWS_ACCURATE_RND;
		}

#if LIBAVFORMAT_VERSION_MAJOR > 56
		m_swsCtx = sws_getCachedContext(nullptr, m_stream->codecpar->width, m_stream->codecpar->height, (AVPixelFormat)m_stream->codecpar->format,
			m_stream->codecpar->width, m_stream->codecpar->height, AV_PIX_FMT_RGBA,
			algorithm, nullptr, nullptr, nullptr);
#else
		m_swsCtx = sws_getCachedContext(nullptr, m_stream->codec->width, m_stream->codec->height, m_stream->codec->pix_fmt,
			m_stream->codec->width, m_stream->codec->height, AV_PIX_FMT_RGBA,
			algorithm, nullptr, nullptr, nullptr);
#endif
	}

	void VideoStream::rescale(AVFrame* frame, uint8_t* outVideoBuffer[4], int outVideoLinesize[4])
	{
		if (frame != nullptr)
		{
			sws_scale(m_swsCtx, frame->data, frame->linesize, 0, frame->height, outVideoBuffer, outVideoLinesize);
		}
	}

	void VideoStream::willPlay(const Timer &timer)
	{
		Stream::willPlay(timer);
		if (getStatus() == Stopped)
		{
			preload();
		}
	}

	sf::Time VideoStream::codecBufferingDelay() const
	{
		sf::Time delay;
		for (const auto& packetDelay : m_codecBufferingDelays)
		{
			delay += packetDelay;
		}
		return delay;
	}
}
