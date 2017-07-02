
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
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include "VideoStream.hpp"
#include "Utilities.hpp"

namespace sfe
{
    VideoStream::VideoStream(AVFormatContext*& formatCtx, AVStream*& stream,
                             DataSource& dataSource, std::shared_ptr<Timer> timer, Delegate& delegate) :
    Stream(formatCtx ,stream, dataSource, timer),
    m_texture(),
    m_rawVideoFrame(nullptr),
    m_rgbaVideoBuffer(),
    m_rgbaVideoLinesize(),
    m_delegate(delegate),
    m_swsCtx(nullptr)
    {
        int err;
        
        for (int i = 0; i < 4;i++)
        {
            m_rgbaVideoBuffer[i] = nullptr;
            m_rgbaVideoLinesize[i] = 0;
        }
        
        m_rawVideoFrame = av_frame_alloc();
        CHECK(m_rawVideoFrame, "VideoStream() - out of memory");
        
        // RGBA video buffer
#if LIBAVFORMAT_VERSION_MAJOR > 56
        err = av_image_alloc(m_rgbaVideoBuffer, m_rgbaVideoLinesize,
                             m_stream->codecpar->width, m_stream->codecpar->height,
                             AV_PIX_FMT_RGBA, 1);
#else
        err = av_image_alloc(m_rgbaVideoBuffer, m_rgbaVideoLinesize,
                             m_stream->codec->width, m_stream->codec->height,
                             AV_PIX_FMT_RGBA, 1);
#endif
        CHECK(err >= 0, "VideoStream() - av_image_alloc() error");
        
        // SFML video frame
#if LIBAVFORMAT_VERSION_MAJOR > 56
        err = m_texture.create(m_stream->codecpar->width, m_stream->codecpar->height);
#else
        err = m_texture.create(m_stream->codec->width, m_stream->codec->height);
#endif
        CHECK(err, "VideoStream() - sf::Texture::create() error");
        
        initRescaler();
    }
    
    VideoStream::~VideoStream()
    {
        if (m_rawVideoFrame)
        {
            av_frame_free(&m_rawVideoFrame);
        }
        
        if (m_rgbaVideoBuffer[0])
        {
            av_freep(&m_rgbaVideoBuffer[0]);
        }
        
        if (m_swsCtx)
        {
            sws_freeContext(m_swsCtx);
        }
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
        while (getStatus() == Playing && (couldComputeGap = getSynchronizationGap(gap)) &&
               gap < sf::Time::Zero)
        {
            if (!onGetData(m_texture))
            {
                setStatus(Stopped);
            }
            else
            {
                static const sf::Time skipFrameThreshold(sf::milliseconds(50));
                if (getSynchronizationGap(gap) && gap + skipFrameThreshold >= sf::Time::Zero)
                    m_delegate.didUpdateVideo(*this, m_texture);
            }
        }
        
        if (! couldComputeGap)
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
        
        while ((couldGetPosition = computeEncodedPosition(position)) && position < targetPosition)
        {
            // We HAVE to decode the frames to get a full image when we reach the target position
            if (! onGetData(m_texture))
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
        
        if (packet)
        {
            goOn = true;
            
            while (!gotFrame && packet && goOn)
            {
                bool needsMoreDecoding = false;
                
                CHECK(packet != nullptr, "inconsistency error");
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
                        m_codecBufferingDelays.pop_front();
#else
                    if (m_codecBufferingDelays.size() > (size_t)m_stream->codec->delay)
                        m_codecBufferingDelays.pop_front();
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
        if (computeEncodedPosition(position))
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
        int gotPicture = 0;
        needsMoreDecoding = false;
        
#if LIBAVCODEC_VERSION_MAJOR > 56
        int decodedLength = avcodec_decode_video2(m_codecCtx, outputFrame, &gotPicture, packet);
#else
        int decodedLength = avcodec_decode_video2(m_stream->codec, outputFrame, &gotPicture, packet);
#endif
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
        CHECK(m_swsCtx, "VideoStream::initRescaler() - sws_getContext() error");
    }
    
    void VideoStream::rescale(AVFrame* frame, uint8_t* outVideoBuffer[4], int outVideoLinesize[4])
    {
        CHECK(frame, "VideoStream::rescale() - invalid argument");
        sws_scale(m_swsCtx, frame->data, frame->linesize, 0, frame->height, outVideoBuffer, outVideoLinesize);
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
        for (const sf::Time& packetDelay : m_codecBufferingDelays)
            delay += packetDelay;
        
        return delay;
    }
}
