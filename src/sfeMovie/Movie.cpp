/*
 *  Movie.cpp
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

#include "Movie.hpp"
#include "MovieImpl.hpp"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace sfe
{
	Movie::Movie() : m_impl(std::make_shared<MovieImpl>(*this)) {}

	bool Movie::openFromFile(const std::string_view filename)
	{
		return m_impl->openFromFile(filename);
	}

	bool Movie::openFromStream(sf::InputStream& stream)
	{
		return m_impl->openFromStream(stream);
	}

	const std::vector<StreamDescriptor>& Movie::getStreams(MediaType type) const
	{
		return m_impl->getStreams(type);
	}

	bool Movie::selectStream(const StreamDescriptor& streamDescriptor)
	{
		return m_impl->selectStream(streamDescriptor);
	}

	void Movie::play()
	{
		m_impl->play();
	}

	void Movie::pause()
	{
		m_impl->pause();
	}

	void Movie::stop()
	{
		m_impl->stop();
	}

	void Movie::update()
	{
		m_impl->update();
	}

	void Movie::setVolume(float volume)
	{
		m_impl->setVolume(volume);
	}

	float Movie::getVolume() const
	{
		return m_impl->getVolume();
	}

	sf::Time Movie::getDuration() const
	{
		return m_impl->getDuration();
	}

	sf::Vector2f Movie::getSize() const
	{
		return m_impl->getSize();
	}

	void Movie::fit(float x, float y, float width, float height, bool preserveRatio)
	{
		m_impl->fit(x, y, width, height, preserveRatio);
	}

	void Movie::fit(sf::FloatRect frame, bool preserveRatio)
	{
		m_impl->fit(frame, preserveRatio);
	}

	float Movie::getFramerate() const
	{
		return m_impl->getFramerate();
	}

	unsigned int Movie::getSampleRate() const
	{
		return m_impl->getSampleRate();
	}

	unsigned int Movie::getChannelCount() const
	{
		return m_impl->getChannelCount();
	}

	Status Movie::getStatus() const
	{
		return m_impl->getStatus();
	}

	sf::Time Movie::getPlayingOffset() const
	{
		return m_impl->getPlayingOffset();
	}

	bool Movie::setPlayingOffset(const sf::Time& targetSeekTime)
	{
		return m_impl->setPlayingOffset(targetSeekTime);
	}

	const sf::Texture& Movie::getCurrentImage() const
	{
		return m_impl->getCurrentImage();
	}

	void Movie::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(*m_impl, states);
	}
} // namespace sfe
