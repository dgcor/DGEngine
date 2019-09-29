/*
 *  AudioStream.hpp
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

#include <cstdint>
#include <SFML/Audio.hpp>
#include "Stream.hpp"

namespace sfe
{
	class AudioStream : public Stream, private sf::SoundStream
	{
	public:
		/** Create an audio stream from the given FFmpeg stream
		 *
		 * At the end of the constructor, the stream is guaranteed
		 * to have all of its fields set and the decoder loaded
		 */
		AudioStream(AVFormatContext*& formatCtx, AVStream*& stream,
			DataSource& dataSource, Timer* timer);

		/** Default destructor
		 */
		~AudioStream() override;

		/** Empty the encoded data queue, destroy all the packets and flush the decoding pipeline
		 */
		void flushBuffers() override;

		/** Get the stream kind (either audio or video stream)
		 *
		 * @return the kind of stream represented by this stream
		 */
		MediaType getStreamKind() const override;

		/** Update the stream's status
		 */
		void update() override;

		/** @see Stream::fastForward()
		 */
		bool fastForward(sf::Time targetPosition) override;

		/** @see Stream::hasError()
		 */
		bool hasError() const noexcept override;

		using sf::SoundStream::setVolume;
		using sf::SoundStream::getVolume;
		using sf::SoundStream::getSampleRate;
		using sf::SoundStream::getChannelCount;
	private:
		bool onGetData(sf::SoundStream::Chunk& data) override;
		void onSeek(sf::Time timeOffset) override;

		/** Decode the encoded data @a packet into @a outputFrame
		 *
		 * gotFrame being set to false means that decoding should still continue:
		 *  - with a new packet if false is returned
		 *    - with the same packet if true is returned
		 *
		 * @param packet the encoded data
		 * @param outputFrame one decoded data
		 * @param gotFrame set to true if a frame has been extracted to outputFrame, false otherwise
		 * @return true if there's still data to decode in this packet, false otherwise
		 */
		bool decodePacket(AVPacket* packet, AVFrame* outputFrame, bool& gotFrame);

		/** Initialize the audio resampler for conversion from many formats to signed 16 bits audio
		 *
		 * This must be called before any packet is decoded and resampled
		 */
		void initResampler();

		/** Resample the decoded audio frame @a frame into signed 16 bits audio samples
		 *
		 * @param frame the audio samples to convert
		 * @param outSamples [out] the convertedSamples
		 * @param outNbSamples [out] the count of signed 16 bits samples in @a outSamples
		 */
		void resampleFrame(const AVFrame* frame, uint8_t*& outSamples, int& outNbSamples);

		/** @return the amount of samples that would last the given time with the current audio stream
		 * properties
		 */
		int timeToSamples(const sf::Time& time) const;

		/** @return the time that would last the given amount of samples with the current audio stream
		 * properties
		 */
		sf::Time samplesToTime(int nbSamples) const;

		// Timer::Observer interface
		void willPlay(const Timer &timer) override;
		void didPlay(const Timer& timer, sfe::Status previousStatus) override;
		void didPause(const Timer& timer, sfe::Status previousStatus) override;
		void didStop(const Timer& timer, sfe::Status previousStatus) override;

		// Public properties
		unsigned m_sampleRatePerChannel{ 0 };

		// Private data
		sf::Int16* m_samplesBuffer{ nullptr };
		AVFrame* m_audioFrame{ nullptr };
		sf::Time m_extraAudioTime;

		// Resampling
		struct SwrContext* m_swrCtx{ nullptr };
		int m_dstNbSamples{ 0 };
		int m_maxDstNbSamples{ 0 };
		int m_dstNbChannels{ 0 };
		int m_dstLinesize{ 0 };
		uint8_t** m_dstData{ nullptr };
	};
}
