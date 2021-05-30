/*
 *  Stream.hpp
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

#include <list>
#include <memory>
#include "Movie.hpp"
#include <SFML/System.hpp>
#include "Timer.hpp"

extern "C"
{
#include <libavformat/avformat.h>
}

namespace sfe
{
	class Stream : public Timer::Observer
	{
	public:
		struct DataSource
		{
			virtual void requestMoreData(Stream& starvingStream) = 0;
			virtual void resetEndOfFileStatus() = 0;
		};

		/** Create a stream from the given FFmpeg stream
		 *
		 * At the end of the constructor, the stream is guaranteed
		 * to have all of its fields set and the decoder loaded
		 *
		 * @param stream the FFmpeg stream
		 * @param dataSource the encoded data provider for this stream
		 */
		Stream(AVFormatContext*& formatCtx, AVStream*& stream, DataSource& dataSource, Timer* timer);

		/** Default destructor
		 */
		virtual ~Stream();

		/** Connect this stream against the reference timer to receive playback events; this allows this
		 * stream to be played
		 */
		void connect();

		/** Disconnect this stream from the reference timer ; this disables this stream
		 */
		void disconnect();

		/** Called by the demuxer to provide the stream with encoded data
		 *
		 * @return packet the encoded data usable by this stream
		 */
		virtual void pushEncodedData(AVPacket* packet);

		/** Reinsert an AVPacket at the beginning of the queue
		 *
		 * This is used for packets that contain several frames, but whose next frames
		 * cannot be decoded yet. These packets are repushed to be decoded when possible.
		 *
		 * @param packet the packet to re-insert at the beginning of the queue
		 */
		virtual void prependEncodedData(AVPacket* packet);

		/** Return the oldest encoded data that was pushed to this stream
		 *
		 * If no packet is stored when this method is called, it will ask the
		 * data source to feed this stream first
		 *
		 * @return the oldest encoded data, or nullptr if no data could be read from the media
		 */
		virtual AVPacket* popEncodedData();

		/** Empty the encoded data queue, destroy all the packets and flush the decoding pipeline
		 * @warning Subclasses overriding this method must also call the Stream implementation
		 */
		virtual void flushBuffers();

		/** Used by the demuxer to know if this stream should be fed with more data
		 *
		 * The default implementation returns true if the packet list contains less than 10 packets
		 *
		 * @return true if the demuxer should give more data to this stream, false otherwise
		 */
		virtual bool needsMoreData() const;

		/** Get the stream kind (either audio or video stream)
		 *
		 * @return the kind of stream represented by this stream
		 */
		virtual MediaType getStreamKind() const;

		/** Give the stream's status
		 *
		 * @return The stream's status (Playing, Paused or Stopped)
		 */
		Status getStatus() const;

		/** Return the stream's language code
		 *
		 * @return the language code of the stream as ISO 639-2 format
		 */
		std::string getLanguage() const;

		/** Compute the stream position in the media, by possibly fetching a packet
		 *
		 * @param[out] position the current stream position, if available
		 * @return true if stream position could be computed and @ref position is set, false otherwise
		 */
		bool computeEncodedPosition(sf::Time& position);

		/** Compute how much time would be covered by the given packet, it's the diff between
		 * the current packet pts, and the next packet pts
		 */
		sf::Time packetDuration(const AVPacket* packet) const;

		/** Discard the data not needed to start playback at the given position
		 *
		 * Every single bit of unneeded data must be discarded as streams synchronization accuracy will
		 * depend on this
		 *
		 * @param targetPosition the position for which the stream is expected to be ready to play
		 * @return true is fast forwarding could be done successfully, false otherwise
		 */
		virtual bool fastForward(sf::Time targetPosition) = 0;

		/** Update the current stream's status and eventually decode frames
		 */
		virtual void update() = 0;

		/** @return true if the given packet is for the current stream
		 */
		bool canUsePacket(AVPacket* packet) const;

		/** @return true if this stream never requests packets and let
		 * itself be fed, false otherwise. Default implementation always
		 * returns false
		 */
		virtual bool isPassive() const;

		/** @return true if this stream wasn't properly constructed.
		 */
		virtual bool hasError() const noexcept;
	protected:
		// Timer::Observer interface
		void didPlay(const Timer& timer, Status previousStatus) override;
		void didPause(const Timer& timer, Status previousStatus) override;
		void didStop(const Timer& timer, Status previousStatus) override;
		bool didSeek(const Timer& timer, sf::Time oldPosition) override;

		/** @return true if any raw packet for the current stream is queued
		 */
		bool hasPackets();

		void setStatus(Status status);

		AVFormatContext*& m_formatCtx;
		AVStream*& m_stream;

		DataSource& m_dataSource;
		Timer* m_timer{ nullptr };
		AVCodec* m_codec{ nullptr };
		AVCodecContext* m_codecCtx{ nullptr };

		int m_streamID{ -1 };
		std::string m_language;
		std::list<AVPacket*> m_packetList;
		Status m_status{ Stopped };
		sf::Mutex m_readerMutex;
	};
}
