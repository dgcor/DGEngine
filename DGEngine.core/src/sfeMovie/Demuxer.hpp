/*
 *  Demuxer.hpp
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

#pragma once

#include <array>
#include "AudioStream.hpp"
#include <list>
#include <memory>
#include <SFML/System.hpp>
#include "Stream.hpp"
#include <string_view>
#include "Timer.hpp"
#include <utility>
#include "VideoStream.hpp"

namespace sfe
{
	class Demuxer : public Stream::DataSource, public Timer::Observer
	{
	public:
		/** Default constructor
		 *
		 * Open a media file and find its streams
		 *
		 * @param sourceFile the path of the media to open and play
		 * @param timer the timer with which the media streams will be synchronized
		 * @param videoDelegate the delegate that will handle the images produced by the VideoStreams
		 */
		Demuxer(const std::string_view sourceFile, Timer* timer, VideoStream::Delegate& videoDelegate);

		/** Default constructor
		*
		* Open a media file from an InputStream and find its streams
		*
		* @param inputStream the InputStream of the media to open and play
		* @param timer the timer with which the media streams will be synchronized
		* @param videoDelegate the delegate that will handle the images produced by the VideoStreams
		*/
		Demuxer(sf::InputStream& inputStream, Timer* timer, VideoStream::Delegate& videoDelegate);

		/** Default destructor
		 */
		~Demuxer() override;

		/** Return a list of audio streams found in the media
		 * The map key is the index of the stream
		 *
		 * @return the list of streams
		 */
		const std::vector<std::pair<std::unique_ptr<AudioStream>, int>>& getAudioStreams() const;

		/** Return a list of video streams found in the media
		 * The map key is the index of the stream
		 *
		 * @return the list of streams
		 */
		const std::vector<std::pair<std::unique_ptr<VideoStream>, int>>& getVideoStreams() const;

		/** Gather the required stream metadata from each stream of the given type
		 *
		 * @param type the type of the streams that are to be described
		 * @return the stream entries computed from the gathered metadata
		 */
		std::vector<StreamDescriptor> computeStreamDescriptors(MediaType type) const;

		/** Enable the given audio stream and connect it to the reference timer
		 *
		 * If another stream of the same kind is already enabled, it is first disabled and disconnected
		 * so that only one stream of the same kind can be enabled at the same time.
		 *
		 * @param stream the audio stream to enable and connect for playing, or nullptr to disable audio
		 */
		void selectAudioStream(AudioStream* stream);

		/** Enable the first found audio stream, if it exists
		 *
		 * @see selectAudioStream
		 */
		void selectFirstAudioStream();

		/** Get the currently selected audio stream, if there's one
		 *
		 * @return the currently selected audio stream, or nullptr if there's none
		 */
		AudioStream* getSelectedAudioStream() const;

		/** Enable the given video stream and connect it to the reference timer
		 *
		 * If another stream of the same kind is already enabled, it is first disabled and disconnected
		 * so that only one stream of the same kind can be enabled at the same time.
		 *
		 * @param stream the video stream to enable and connect for playing, or nullptr to disable video
		 */
		void selectVideoStream(VideoStream* stream);

		/** Enable the first found video stream, if it exists
		 *
		 * @see selectAudioStream
		 */
		void selectFirstVideoStream();

		/** Get the currently selected video stream, if there's one
		 *
		 * @return the currently selected video stream, or nullptr if there's none
		 */
		VideoStream* getSelectedVideoStream() const;

		/** Read encoded data from the media and makes sure that the given stream
		 * has enough data
		 *
		 * @param stream The stream to feed
		 */
		void feedStream(Stream& stream);

		/** Update the media status and eventually decode frames
		 */
		void update();

		/** Tell whether the demuxer has reached the end of the file and can no more feed the streams
		 *
		 * @return whether the end of the media file has been reached
		 */
		bool didReachEndOfFile() const;

		/** Give the media duration
		 *
		 * @return the media duration
		 */
		sf::Time getDuration() const;

	private:

		static const int kBufferSize = 1024 * 1024 * 16;

		class InputStreamIOContext
		{
		private:
			InputStreamIOContext(InputStreamIOContext const &) = delete;
			InputStreamIOContext& operator=(InputStreamIOContext const &) = delete;

		public:
			// Move constructor
			InputStreamIOContext(InputStreamIOContext&& other) noexcept
			{
				m_inputStream = other.m_inputStream;
				m_ctx = other.m_ctx;
				other.m_ctx = nullptr;
			}

			// Move assignment operator
			InputStreamIOContext& operator=(InputStreamIOContext&& other) noexcept
			{
				m_inputStream = other.m_inputStream;
				m_ctx = other.m_ctx;
				other.m_ctx = nullptr;
				return *this;
			}

			InputStreamIOContext() : m_inputStream(nullptr), m_ctx(nullptr) {}

			InputStreamIOContext(sf::InputStream* inputStream) : m_inputStream(inputStream)
			{
				unsigned char* buffer = static_cast<unsigned char*>(::av_malloc(kBufferSize));
				m_ctx = ::avio_alloc_context(buffer, kBufferSize, 0, this,
					&InputStreamIOContext::read, nullptr, &InputStreamIOContext::seek);
			}

			~InputStreamIOContext()
			{
				if (m_ctx != nullptr)
				{
					auto buf = m_ctx->buffer;
					::av_freep(&m_ctx);
					::av_free(buf);
				}
			}

			static int read(void* opaque, unsigned char* buf, int buf_size)
			{
				InputStreamIOContext* h = static_cast<InputStreamIOContext*>(opaque);
				auto readBytes = h->m_inputStream->read(buf, buf_size);
				if (readBytes == 0)
				{
					return AVERROR_EOF;
				}
				return (int)readBytes;
			}

			static int64_t seek(void* opaque, int64_t offset, int whence)
			{
				InputStreamIOContext* h = static_cast<InputStreamIOContext*>(opaque);
				if (whence == AVSEEK_SIZE)
				{
					return h->m_inputStream->getSize();
				}
				return h->m_inputStream->seek(offset);
			}

			::AVIOContext* getAVIOContext() { return m_ctx; }

		private:
			sf::InputStream* m_inputStream;
			::AVIOContext* m_ctx;
		};

		/** Common Constructor
		*/
		void init(const char* fileName, VideoStream::Delegate& videoDelegate);

		/** Read a encoded packet from the media file
		 *
		 * You're responsible for freeing the returned packet
		 *
		 * @return the read packet, or nullptr if the end of file has been reached
		 */
		AVPacket* readPacket();

		/** Empty the temporarily encoded data queue
		 */
		void flushBuffers();

		/** Empty the temporarily encoded data queue
		 */
		void flushBuffer(std::list<AVPacket*>& buffer);

		/** Queue a packet that has been read and is to be used by an active stream in near future
		 *
		 * @param packet the packet to temporarily store
		 */
		void queueEncodedData(AVPacket* packet);

		/** Check whether data that should be distributed to the given stream is currently pending
		 * in the demuxer's temporary queue
		 *
		 * @param stream the stream for which pending data availability is to be checked
		 * @param whether pending data exists for the given stream
		 */
		bool hasPendingDataForStream(const Stream& stream) const;

		/** Look for a queued packet for the given stream
		 *
		 * @param stream the stream for which to search a packet
		 * @return if a packet for the given stream has been found, it is dequeued and returned
		 * otherwise nullptr is returned
		 */
		AVPacket* gatherQueuedPacketForStream(Stream& stream);

		/** Distribute the given packet to the correct stream
		 *
		 * If the packet doesn't match any known stream, nothing is done
		 *
		 * @param packet the packet to distribute
		 * @param stream the stream that requested data from the demuxer, if the packet is not for this stream
		 * it must be queued
		 * @return true if the packet could be distributed, false otherwise
		 */
		bool distributePacket(AVPacket* packet, Stream& stream);

		/** Try to extract the media duration from the given stream
		 */
		void extractDurationFromStream(const AVStream* stream);

		// Data source interface
		void requestMoreData(Stream& starvingStream) override;
		void resetEndOfFileStatus() override;

		// Timer interface
		bool didSeek(const Timer& timer, sf::Time oldPosition) override;

		static constexpr size_t AudioStreamIndex = 0;
		static constexpr size_t VideoStreamIndex = 1;

		std::unique_ptr<InputStreamIOContext> m_streamContext;
		AVFormatContext* m_formatCtx{ nullptr };
		bool m_eofReached{ false };
		std::vector<std::pair<std::unique_ptr<AudioStream>, int>> m_audioStreams;
		std::vector<std::pair<std::unique_ptr<VideoStream>, int>> m_videoStreams;
		mutable sf::Mutex m_synchronized;
		Timer* m_timer{ nullptr };
		std::array<Stream*, 2> m_connectedStreams{};
		sf::Time m_duration;
		std::array<std::pair<Stream*, std::list<AVPacket*>>, 2> m_pendingDataForActiveStreams{};
	};
}
