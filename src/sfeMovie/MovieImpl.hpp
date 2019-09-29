/*
 *  MovieImpl.hpp
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

#include <memory>
#include <SFML/Graphics/Sprite.hpp>
#include <string_view>
#include "VideoStream.hpp"

namespace sfe
{
	class Demuxer;

	class MovieImpl : public VideoStream::Delegate, public sf::Drawable
	{
	public:
		MovieImpl(sf::Transformable& movieView);
		~MovieImpl() override;

		/** @see Movie::openFromFile()
		 */
		bool openFromFile(const std::string_view filename);

		/** @see Movie::openFromStream()
		*/
		bool openFromStream(sf::InputStream& stream);

		/** @see Movie::getStreams()
		 */
		const std::vector<StreamDescriptor>& getStreams(MediaType type) const;

		/** @see Movie::selectStream()
		 */
		bool selectStream(const StreamDescriptor& streamDescriptor);

		/** @see Movie::play()
		 */
		void play();

		/** @see Movie::pause()
		 */
		void pause();

		/** @see Movie::stop()
		 */
		void stop();

		/** Update the media status and eventually decode frames
		 */
		void update();

		/** @see Movie::setVolume()
		 */
		void setVolume(float volume);

		/** @see Movie::getVolume()
		 */
		float getVolume() const;

		/** @see Movie::getDuration()
		 */
		sf::Time getDuration() const;

		/** @see Movie::getSize()
		 */
		sf::Vector2f getSize() const;

		/** @see fit(sf::FloatRect, bool)
		 */
		void fit(float x, float y, float width, float height, bool preserveRatio = true);

		/** @see Movie::fit()
		 */
		void fit(sf::FloatRect frame, bool preserveRatio = true);

		/** @see Movie::getFramerate()
		 */
		float getFramerate() const;

		/** @see Movie::getSampleRate()
		 */
		unsigned int getSampleRate() const;

		/** @see Movie::getChannelCount()
		 */
		unsigned int getChannelCount() const;

		/** @see Movie::getStatus()
		 */
		Status getStatus() const;

		/** @see Movie::getPlayingOffset()
		 */
		sf::Time getPlayingOffset() const;

		/** @see Movie::setPlayingOffset()
		 */
		bool setPlayingOffset(const sf::Time& targetSeekTime);

		/** @see Movie::getCurrentImage()
		 */
		const sf::Texture& getCurrentImage() const;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void didUpdateVideo(const VideoStream& sender, const sf::Texture& image) override;

	private:

		bool open();

		sf::Transformable& m_movieView;
		std::unique_ptr<Demuxer> m_demuxer;
		std::unique_ptr<Timer> m_timer;
		sf::Sprite m_videoSprite;
		std::vector<StreamDescriptor> m_audioStreamsDesc;
		std::vector<StreamDescriptor> m_videoStreamsDesc;
		std::vector<StreamDescriptor> m_noStreamsDesc;
		sf::FloatRect m_displayFrame;
	};
}
