
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
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <stdint.h>
}

#include "Demuxer.hpp"
#include "VideoStream.hpp"
#include "AudioStream.hpp"
#include "Utilities.hpp"
#include "TimerPriorities.hpp"
#include <iostream>
#include <stdexcept>

namespace sfe
{
    std::list<Demuxer::DemuxerInfo> Demuxer::g_availableDemuxers;
    std::list<Demuxer::DecoderInfo> Demuxer::g_availableDecoders;
    
    static void loadFFmpeg()
    {
        ONCE(av_register_all());
        ONCE(avcodec_register_all());
    }
    
    static MediaType AVMediaTypeToMediaType(AVMediaType type)
    {
        switch (type)
        {
            case AVMEDIA_TYPE_AUDIO:    return Audio;
            case AVMEDIA_TYPE_VIDEO:    return Video;
            default:                    return Unknown;
        }
    }
    
    const std::list<Demuxer::DemuxerInfo>& Demuxer::getAvailableDemuxers()
    {
        AVInputFormat* demuxer = nullptr;
        loadFFmpeg();
        
        if (g_availableDemuxers.empty())
        {
            while (nullptr != (demuxer = av_iformat_next(demuxer)))
            {
                DemuxerInfo info =
                {
                    std::string(demuxer->name),
                    std::string(demuxer->long_name)
                };
                
                g_availableDemuxers.push_back(info);
            }
        }
        
        return g_availableDemuxers;
    }
    
    const std::list<Demuxer::DecoderInfo>& Demuxer::getAvailableDecoders()
    {
        AVCodec* codec = nullptr;
        loadFFmpeg();
        
        if (g_availableDecoders.empty())
        {
            while (nullptr != (codec = av_codec_next(codec)))
            {
                DecoderInfo info =
                {
                    avcodec_get_name(codec->id),
                    codec->long_name,
                    AVMediaTypeToMediaType(codec->type)
                };
                
                g_availableDecoders.push_back(info);
            }
        }
        
        return g_availableDecoders;
    }
    
    Demuxer::Demuxer(const std::string& sourceFile, std::shared_ptr<Timer> timer,
                     VideoStream::Delegate& videoDelegate) :
    m_streamContext(),
    m_formatCtx(nullptr),
    m_eofReached(false),
    m_streams(),
    m_synchronized(),
    m_timer(timer),
    m_connectedAudioStream(nullptr),
    m_connectedVideoStream(nullptr),
    m_duration(sf::Time::Zero)
    {
        CHECK(sourceFile.size(), "Demuxer::Demuxer() - invalid argument: sourceFile");
        CHECK(timer, "Inconsistency error: null timer");
        
        // Load all the decoders
        loadFFmpeg();
        
        init(sourceFile.c_str(), videoDelegate);
    }
    
    Demuxer::Demuxer(sf::InputStream& inputStream, std::shared_ptr<Timer> timer,
                     VideoStream::Delegate& videoDelegate) :
    m_streamContext(),
    m_formatCtx(nullptr),
    m_eofReached(false),
    m_streams(),
    m_synchronized(),
    m_timer(timer),
    m_connectedAudioStream(nullptr),
    m_connectedVideoStream(nullptr),
    m_duration(sf::Time::Zero)
    {
        CHECK(timer, "Inconsistency error: null timer");
        
        // Load all the decoders
        loadFFmpeg();
        
        m_streamContext = InputStreamIOContext(&inputStream);
        m_formatCtx = ::avformat_alloc_context();
        m_formatCtx->pb = m_streamContext.getAVIOContext();
        init("", videoDelegate);
    }
    
    void Demuxer::init(const char* fileName, VideoStream::Delegate& videoDelegate)
    {
        int err = 0;
        
        // Open the movie file
        err = avformat_open_input(&m_formatCtx, fileName, nullptr, nullptr);
        CHECK0(err, "Demuxer::Demuxer() - error while opening media stream");
        CHECK(m_formatCtx, "Demuxer() - inconsistency: media context cannot be nullptr");
        
        // Read the general movie informations
        err = avformat_find_stream_info(m_formatCtx, nullptr);
        CHECK0(err, "Demuxer::Demuxer() - error while retreiving media information");
        
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
            
            try
            {
                std::shared_ptr<Stream> stream;
                
#if LIBAVFORMAT_VERSION_MAJOR > 56
                switch (ffstream->codecpar->codec_type)
#else
                switch (ffstream->codec->codec_type)
#endif
                {
                    case AVMEDIA_TYPE_VIDEO:
                        stream = std::make_shared<VideoStream>(m_formatCtx, ffstream, *this, m_timer, videoDelegate);
                        
                        if (m_duration == sf::Time::Zero)
                        {
                            extractDurationFromStream(ffstream);
                        }
                        break;
                        
                    case AVMEDIA_TYPE_AUDIO:
                        stream = std::make_shared<AudioStream>(m_formatCtx, ffstream, *this, m_timer);
                        
                        if (m_duration == sf::Time::Zero)
                        {
                            extractDurationFromStream(ffstream);
                        }
                        break;
                    default:
                        break;
                }
                
                // Don't create an entry in the map unless everything went well and stream did not get ignored
                if (stream)
                    m_streams[ffstream->index] = stream;
            }
            catch (std::runtime_error)
            {
                CHECK(m_streams.find(ffstream->index) == m_streams.end(),
                      "Internal inconcistency error: stream whose loading failed should not be stored");
            }
        }
        
        m_timer->addObserver(*this, DemuxerTimerPriority);
    }
    
    Demuxer::~Demuxer()
    {
        if (m_timer->getStatus() != Stopped)
            m_timer->stop();
        
        m_timer->removeObserver(*this);
        
        // NB: these manual cleaning are important for the AVFormatContext to be deleted last, otherwise
        // the streams lose their connection to the codec and leak
        m_streams.clear();
        m_connectedAudioStream.reset();
        m_connectedVideoStream.reset();
        
        if (m_formatCtx)
        {
            // Be very careful with this call: it'll also destroy its codec contexts and streams
            avformat_close_input(&m_formatCtx);
        }
        
        flushBuffers();
    }
    
    const std::map<int, std::shared_ptr<Stream> >& Demuxer::getStreams() const
    {
        return m_streams;
    }
    
    std::set< std::shared_ptr<Stream> > Demuxer::getStreamsOfType(MediaType type) const
    {
        std::set< std::shared_ptr<Stream> > streamSet;
        
        for (const std::pair<int, std::shared_ptr<Stream> >& pair : m_streams)
        {
            if (pair.second->getStreamKind() == type)
                streamSet.insert(pair.second);
        }
        
        return streamSet;
    }
    
    Streams Demuxer::computeStreamDescriptors(MediaType type) const
    {
        Streams entries;
        std::set< std::shared_ptr<Stream> > streamSet;
        
        for (const std::pair<int, std::shared_ptr<Stream> >& pair : m_streams)
        {
            if (pair.second->getStreamKind() == type)
            {
                StreamDescriptor entry;
                entry.type = type;
                entry.identifier = pair.first;
                entry.language = pair.second->getLanguage();
                entries.push_back(entry);
            }
        }
        
        return entries;
    }
    
    void Demuxer::selectAudioStream(std::shared_ptr<AudioStream> stream)
    {
        Status oldStatus = m_timer->getStatus();
        CHECK(oldStatus == Stopped, "Changing the selected stream after starting "
              "the movie playback isn't supported yet");
        
        if (oldStatus == Playing)
            m_timer->pause();
        
        if (stream != m_connectedAudioStream)
        {
            if (m_connectedAudioStream)
            {
                m_connectedAudioStream->disconnect();
            }
            
            if (stream)
                stream->connect();
            
            m_connectedAudioStream = stream;
        }
        
        if (oldStatus == Playing)
            m_timer->play();
    }
    
    void Demuxer::selectFirstAudioStream()
    {
        std::set< std::shared_ptr<Stream> > audioStreams = getStreamsOfType(Audio);
        if (audioStreams.size())
            selectAudioStream(std::dynamic_pointer_cast<AudioStream>(*audioStreams.begin()));
    }
    
    std::shared_ptr<AudioStream> Demuxer::getSelectedAudioStream() const
    {
        return std::dynamic_pointer_cast<AudioStream>(m_connectedAudioStream);
    }
    
    void Demuxer::selectVideoStream(std::shared_ptr<VideoStream> stream)
    {
        Status oldStatus = m_timer->getStatus();
        CHECK(oldStatus == Stopped, "Changing the selected stream after starting "
              "the movie playback isn't supported yet");
        
        if (oldStatus == Playing)
            m_timer->pause();
        
        if (stream != m_connectedVideoStream)
        {
            if (m_connectedVideoStream)
            {
                m_connectedVideoStream->disconnect();
            }
            
            if (stream)
                stream->connect();
            
            m_connectedVideoStream = stream;
        }
        
        if (oldStatus == Playing)
            m_timer->play();
    }
    
    void Demuxer::selectFirstVideoStream()
    {
        std::set< std::shared_ptr<Stream> > videoStreams = getStreamsOfType(Video);
        if (videoStreams.size())
            selectVideoStream(std::dynamic_pointer_cast<VideoStream>(*videoStreams.begin()));
    }
    
    std::shared_ptr<VideoStream> Demuxer::getSelectedVideoStream() const
    {
        return std::dynamic_pointer_cast<VideoStream>(m_connectedVideoStream);
    }
    
    void Demuxer::feedStream(Stream& stream)
    {
        CHECK(! stream.isPassive(), "Internal inconcistency - Cannot feed a passive stream");
        
        sf::Lock l(m_synchronized);
        
        while ((!didReachEndOfFile() || hasPendingDataForStream(stream)) && stream.needsMoreData())
        {
            AVPacket* pkt = NULL;
            
            pkt = gatherQueuedPacketForStream(stream);
            
            if (!pkt)
                pkt = readPacket();
            
            if (!pkt)
            {
                m_eofReached = true;
            }
            else
            {
                if (!distributePacket(pkt, stream))
                {
#if LIBAVCODEC_VERSION_MAJOR > 56
                    av_packet_unref(pkt);
#else
                    av_free_packet(pkt);
#endif
                    av_free(pkt);
                }
            }
        }
    }
    
    std::set<std::shared_ptr<Stream>> Demuxer::getSelectedStreams() const
    {
        std::set<std::shared_ptr<Stream>> set;
        
        if (m_connectedVideoStream)
            set.insert(m_connectedVideoStream);
        
        if (m_connectedAudioStream)
            set.insert(m_connectedAudioStream);
        
        return set;
    }
    
    void Demuxer::update()
    {
        std::map<int, std::shared_ptr<Stream> > streams = getStreams();
        
        for(std::pair<int, std::shared_ptr<Stream> > pair : streams)
        {
            pair.second->update();
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
        
        AVPacket *pkt = nullptr;
        int err = 0;
        
        pkt = (AVPacket *)av_malloc(sizeof(*pkt));
        CHECK(pkt, "Demuxer::readPacket() - out of memory");
        av_init_packet(pkt);
        
        err = av_read_frame(m_formatCtx, pkt);
        
        if (err < 0)
        {
#if LIBAVCODEC_VERSION_MAJOR > 56
            av_packet_unref(pkt);
#else
            av_free_packet(pkt);
#endif
            av_free(pkt);
            pkt = nullptr;
        }
        
        return pkt;
    }
    
    void Demuxer::flushBuffers()
    {
        sf::Lock l(m_synchronized);
        
        for (std::pair<const Stream*, std::list<AVPacket*> >&& pair : m_pendingDataForActiveStreams)
        {
            for (AVPacket* packet : pair.second)
            {
#if LIBAVCODEC_VERSION_MAJOR > 56
                av_packet_unref(packet);
#else
                av_free_packet(packet);
#endif
                av_free(packet);
            }
        }
        
        m_pendingDataForActiveStreams.clear();
    }
    
    void Demuxer::queueEncodedData(AVPacket* packet)
    {
        sf::Lock l(m_synchronized);
        
        std::set<std::shared_ptr<Stream>> connectedStreams = getSelectedStreams();
        
        for (std::shared_ptr<Stream> stream : connectedStreams)
        {
            if (stream->canUsePacket(packet))
            {
                std::list<AVPacket*>& packets = m_pendingDataForActiveStreams[stream.get()];
                packets.push_back(packet);
                return;
            }
        }
        
#if LIBAVCODEC_VERSION_MAJOR > 56
        av_packet_unref(packet);
#else
        av_free_packet(packet);
#endif
        av_free(packet);
    }
    
    bool Demuxer::hasPendingDataForStream(const Stream& stream) const
    {
        sf::Lock l(m_synchronized);
        
        const std::map<const Stream*, std::list<AVPacket*> >::const_iterator it =
            m_pendingDataForActiveStreams.find(&stream);
        
        if (it != m_pendingDataForActiveStreams.end())
            return ! it->second.empty();
        
        return false;
    }
    
    AVPacket* Demuxer::gatherQueuedPacketForStream(Stream& stream)
    {
        sf::Lock l(m_synchronized);
        
        std::map<const Stream*, std::list<AVPacket*> >::iterator it
            = m_pendingDataForActiveStreams.find(&stream);
        
        if (it != m_pendingDataForActiveStreams.end())
        {
            std::list<AVPacket*>& pendingPackets = it->second;
            
            if (! pendingPackets.empty())
            {
                AVPacket* packet = pendingPackets.front();
                pendingPackets.pop_front();
                return packet;
            }
        }
        
        return NULL;
    }
    
    bool Demuxer::distributePacket(AVPacket* packet, Stream& stream)
    {
        sf::Lock l(m_synchronized);
        CHECK(packet, "Demuxer::distributePacket() - invalid argument");
        
        bool distributed = false;
        std::map<int, std::shared_ptr<Stream> >::iterator it = m_streams.find(packet->stream_index);
        
        if (it != m_streams.end())
        {
            std::shared_ptr<Stream>  targetStream = it->second;
            
            // We don't want to store the packets for inactive streams,
            // let them be freed
            if (targetStream == getSelectedVideoStream() ||
                targetStream == getSelectedAudioStream())
            {
                if (targetStream.get() == &stream || targetStream->isPassive())
                    targetStream->pushEncodedData(packet);
                else
                    queueEncodedData(packet);
                
                distributed = true;
            }
        }
        
        return distributed;
    }
    
    void Demuxer::extractDurationFromStream(const AVStream* stream)
    {
        if (m_duration != sf::Time::Zero)
            return;
        
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
        CHECK(! starvingStream.isPassive(), "Internal inconcistency - passive streams cannot request data");
        
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
        std::set< std::shared_ptr<Stream> > connectedStreams;
        
        if (m_connectedVideoStream)
            connectedStreams.insert(m_connectedVideoStream);
        if (m_connectedAudioStream)
            connectedStreams.insert(m_connectedAudioStream);
        
        CHECK(!connectedStreams.empty(), "Inconcistency error: seeking with no active stream");
        
        // Trivial seeking to beginning
        if (newPosition == sf::Time::Zero)
        {
            int64_t timestamp = 0;
            
            if (m_formatCtx->iformat->flags & AVFMT_SEEK_TO_PTS && m_formatCtx->start_time != AV_NOPTS_VALUE)
                timestamp += m_formatCtx->start_time;
            
            
            // Flush all streams
            for (std::shared_ptr<Stream> stream : connectedStreams)
                stream->flushBuffers();
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
            int64_t timestamp = ((int)newPosition.asSeconds()) * AV_TIME_BASE;
            
            // < 0 = before seek point
            // > 0 = after seek point
            std::map< std::shared_ptr<Stream>, sf::Time> seekingGaps;
            
            static const float brokenSeekingThreshold = 60.f; // seconds
            bool didReseekBackward = false;
            bool didReseekForward = false;
            int tooEarlyCount = 0;
            int tooLateCount = 0;
            int brokenSeekingCount = 0;
            int ffmpegSeekFlags = AVSEEK_FLAG_BACKWARD;
            
            do
            {
                // Flush all streams
                for (std::shared_ptr<Stream> stream : connectedStreams)
                    stream->flushBuffers();
                flushBuffers();
                
                // Seek to new estimated target
                if (m_formatCtx->iformat->flags & AVFMT_SEEK_TO_PTS && m_formatCtx->start_time != AV_NOPTS_VALUE)
                    timestamp += m_formatCtx->start_time;
                
                int err = avformat_seek_file(m_formatCtx, -1, timestamp - 10 * AV_TIME_BASE,
                                             timestamp, timestamp, ffmpegSeekFlags);
                CHECK0(err, "avformat_seek_file failure");
                
                // Compute the new gap
                for (std::shared_ptr<Stream> stream : connectedStreams)
                {
                    if (stream->isPassive())
                        continue;
                    
                    sf::Time position;
                    if (stream->computeEncodedPosition(position))
                    {
                        seekingGaps[stream] = position - newPosition;
                    }
                }
                
                tooEarlyCount = 0;
                tooLateCount = 0;
                brokenSeekingCount = 0;
                
                // Check the current situation
                for (std::pair< std::shared_ptr<Stream>, sf::Time>&& gapByStream : seekingGaps)
                {
                    // < 0 = before seek point
                    // > 0 = after seek point
                    const sf::Time& gap = gapByStream.second;
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
                            brokenSeekingCount++;
                    }
                }
                
                CHECK(false == (tooEarlyCount && tooLateCount),
                      "Both too late and too early for different streams, unhandled situation!");
                
                // Define what to do next
                if (tooEarlyCount)
                {
                    // Go forward by 1 sec
                    timestamp += AV_TIME_BASE;
                    didReseekForward = true;
                }
                else if (tooLateCount)
                {
                    // Go backward by 1 sec
                    timestamp -= AV_TIME_BASE;
                    didReseekBackward = true;
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
                
                CHECK(!(didReseekBackward && didReseekForward), "infinitely seeking backward and forward");
            }
            while (tooEarlyCount != 0 || tooLateCount != 0);
        }
        
        return true;
    }
}
