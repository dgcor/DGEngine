/*
 *  MovieImpl.cpp
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

#include "MovieImpl.hpp"
#include <cmath>
#include "Demuxer.hpp"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Timer.hpp"

namespace sfe
{
	MovieImpl::MovieImpl(sf::Transformable& movieView) : m_movieView(movieView) {}

	MovieImpl::~MovieImpl()
	{
		m_demuxer = nullptr;
		if (m_timer != nullptr && m_timer->getStatus() != Stopped)
		{
			stop();
		}
		m_timer = nullptr;
	}

	bool MovieImpl::openFromFile(const std::string_view filename)
	{
		m_timer = std::make_unique<Timer>();
		m_demuxer = std::make_unique<Demuxer>(filename, m_timer.get(), *this);
		return open();
	}

	bool MovieImpl::openFromStream(sf::InputStream& stream)
	{
		m_timer = std::make_unique<Timer>();
		m_demuxer = std::make_unique<Demuxer>(stream, m_timer.get(), *this);
		return open();
	}

	bool MovieImpl::open()
	{
		if (m_demuxer->getAudioStreams().empty() == true &&
			m_demuxer->getVideoStreams().empty() == true)
		{
			return false;
		}

		m_audioStreamsDesc = m_demuxer->computeStreamDescriptors(Audio);
		m_videoStreamsDesc = m_demuxer->computeStreamDescriptors(Video);

		m_demuxer->selectFirstAudioStream();
		m_demuxer->selectFirstVideoStream();

		if (m_demuxer->getVideoStreams().empty() == false)
		{
			sf::Vector2f size = getSize();
			m_displayFrame = sf::FloatRect(0, 0, size.x, size.y);
		}
		return true;
	}

	const std::vector<StreamDescriptor>& MovieImpl::getStreams(MediaType type) const
	{
		switch (type)
		{
		case Audio:
			return m_audioStreamsDesc;
		case Video:
			return m_videoStreamsDesc;
		default:
			return m_noStreamsDesc;
		}
	}

	bool MovieImpl::selectStream(const StreamDescriptor& streamDescriptor)
	{
		if (m_demuxer == nullptr || m_timer == nullptr)
		{
			return false;
		}
		if (m_timer->getStatus() != Stopped)
		{
			return false;
		}
		switch (streamDescriptor.type)
		{
		case Audio:
		{
			for (const auto& stream : m_demuxer->getAudioStreams())
			{
				if (stream.second == streamDescriptor.identifier)
				{
					m_demuxer->selectAudioStream(dynamic_cast<AudioStream*>(stream.first.get()));
					return true;
				}
			}
			break;
		}
		case Video:
		{
			for (const auto& stream : m_demuxer->getVideoStreams())
			{
				if (stream.second == streamDescriptor.identifier)
				{
					m_demuxer->selectVideoStream(dynamic_cast<VideoStream*>(stream.first.get()));
					return true;
				}
			}
			break;
		}
		default:
			break;
		}
		return false;
	}

	void MovieImpl::play()
	{
		if (m_demuxer == nullptr || m_timer == nullptr)
		{
			return;
		}
		if (m_timer->getStatus() == Playing)
		{
			return;
		}
		m_timer->play();
		update();
	}

	void MovieImpl::pause()
	{
		if (m_demuxer == nullptr || m_timer == nullptr)
		{
			return;
		}
		if (m_timer->getStatus() == Paused)
		{
			return;
		}
		m_timer->pause();
		update();
	}

	void MovieImpl::stop()
	{
		if (m_demuxer == nullptr || m_timer == nullptr)
		{
			return;
		}
		if (m_timer->getStatus() == Stopped)
		{
			return;
		}
		m_timer->stop();
		update();
		auto videoStream = m_demuxer->getSelectedVideoStream();
		if (videoStream)
		{
			videoStream->preload();
		}
	}

	void MovieImpl::update()
	{
		if (m_demuxer == nullptr || m_timer == nullptr)
		{
			return;
		}
		m_demuxer->update();
		if (getStatus() == Stopped && m_timer->getStatus() != Stopped)
		{
			m_timer->stop();
		}
		// Enable smoothing when the video is scaled
		auto vStream = m_demuxer->getSelectedVideoStream();
		if (vStream != nullptr)
		{
			sf::Vector2f movieScale = m_movieView.getScale();
			sf::Vector2f subviewScale = m_videoSprite.getScale();

			if (std::fabs(movieScale.x - 1.f) < 0.00001 &&
				std::fabs(movieScale.y - 1.f) < 0.00001 &&
				std::fabs(subviewScale.x - 1.f) < 0.00001 &&
				std::fabs(subviewScale.y - 1.f) < 0.00001)
			{
				vStream->getVideoTexture().setSmooth(false);
			}
			else
			{
				vStream->getVideoTexture().setSmooth(true);
			}
		}
	}

	void MovieImpl::setVolume(float volume)
	{
		if (m_demuxer == nullptr || m_timer == nullptr)
		{
			return;
		}
		for (auto& stream : m_demuxer->getAudioStreams())
		{
			stream.first->setVolume(volume);
		}
	}

	float MovieImpl::getVolume() const
	{
		if (m_demuxer != nullptr && m_timer != nullptr)
		{
			auto audioStream = m_demuxer->getSelectedAudioStream();
			if (audioStream != nullptr)
			{
				return audioStream->getVolume();
			}
		}
		return 0.f;
	}

	sf::Time MovieImpl::getDuration() const
	{
		if (m_demuxer != nullptr && m_timer != nullptr)
		{
			return m_demuxer->getDuration();
		}
		return sf::Time::Zero;
	}

	sf::Vector2f MovieImpl::getSize() const
	{
		if (m_demuxer != nullptr && m_timer != nullptr)
		{
			auto videoStream = m_demuxer->getSelectedVideoStream();
			if (videoStream != nullptr)
			{
				return sf::Vector2f(videoStream->getFrameSize());
			}
		}
		return sf::Vector2f(0, 0);
	}

	void MovieImpl::fit(float x, float y, float width, float height, bool preserveRatio)
	{
		fit(sf::FloatRect(x, y, width, height), preserveRatio);
	}

	void MovieImpl::fit(sf::FloatRect frame, bool preserveRatio)
	{
		sf::Vector2f movie_size = getSize();

		if (movie_size.x == 0 || movie_size.y == 0)
		{
			return;
		}

		sf::Vector2f wanted_size = sf::Vector2f(frame.width, frame.height);
		sf::Vector2f new_size;

		if (preserveRatio)
		{
			sf::Vector2f target_size = movie_size;

			float source_ratio = movie_size.x / movie_size.y;
			float target_ratio = wanted_size.x / wanted_size.y;

			if (source_ratio > target_ratio)
			{
				target_size.x = movie_size.x * wanted_size.x / movie_size.x;
				target_size.y = movie_size.y * wanted_size.x / movie_size.x;
			}
			else
			{
				target_size.x = movie_size.x * wanted_size.y / movie_size.y;
				target_size.y = movie_size.y * wanted_size.y / movie_size.y;
			}

			new_size = target_size;
		}
		else
		{
			new_size = wanted_size;
		}

		m_videoSprite.setPosition((wanted_size.x - new_size.x) / 2.f,
			(wanted_size.y - new_size.y) / 2.f);
		m_movieView.setPosition(frame.left, frame.top);
		m_videoSprite.setScale((float)new_size.x / movie_size.x, (float)new_size.y / movie_size.y);
		m_displayFrame = frame;
	}

	float MovieImpl::getFramerate() const
	{
		if (m_demuxer != nullptr && m_timer != nullptr)
		{
			auto videoStream = m_demuxer->getSelectedVideoStream();
			if (videoStream != nullptr)
			{
				return videoStream->getFrameRate();
			}
		}
		return 0.f;
	}

	unsigned int MovieImpl::getSampleRate() const
	{
		if (m_demuxer != nullptr && m_timer != nullptr)
		{
			auto audioStream = m_demuxer->getSelectedAudioStream();
			if (audioStream != nullptr)
			{
				return audioStream->getSampleRate();
			}
		}
		return 0;
	}

	unsigned int MovieImpl::getChannelCount() const
	{
		if (m_demuxer != nullptr && m_timer != nullptr)
		{
			auto audioStream = m_demuxer->getSelectedAudioStream();
			if (audioStream != nullptr)
			{
				return audioStream->getChannelCount();
			}
		}
		return 0;
	}

	Status MovieImpl::getStatus() const
	{
		Status st = Stopped;
		if (m_demuxer != nullptr)
		{
			auto videoStream = m_demuxer->getSelectedVideoStream();
			auto audioStream = m_demuxer->getSelectedAudioStream();
			Status vStatus = (videoStream != nullptr ? videoStream->getStatus() : Stopped);
			Status aStatus = (audioStream != nullptr ? audioStream->Stream::getStatus() : Stopped);

			if (vStatus == Playing || aStatus == Playing)
			{
				st = Playing;
			}
			else if (vStatus == Paused || aStatus == Paused)
			{
				st = Paused;
			}
		}
		return st;
	}

	sf::Time MovieImpl::getPlayingOffset() const
	{
		if (m_demuxer != nullptr && m_timer != nullptr)
		{
			return m_timer->getOffset();
		}
		return sf::Time::Zero;
	}

	bool MovieImpl::setPlayingOffset(const sf::Time& targetSeekTime)
	{
		bool seekingResult = false;
		if (m_demuxer != nullptr && m_timer != nullptr)
		{
			if (targetSeekTime >= sf::Time::Zero &&
				targetSeekTime < getDuration())
			{
				seekingResult = m_timer->seek(targetSeekTime);
				if (m_timer->getStatus() == Status::Stopped)
				{
					pause();
				}
			}
		}
		return seekingResult;
	}

	const sf::Texture& MovieImpl::getCurrentImage() const
	{
		static sf::Texture emptyTexture;

		if (m_videoSprite.getTexture() != nullptr)
		{
			return *m_videoSprite.getTexture();
		}
		else
		{
			return emptyTexture;
		}
	}

	void MovieImpl::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_videoSprite, states);
	}

	void MovieImpl::didUpdateVideo(const VideoStream& sender, const sf::Texture& image)
	{
		if (m_videoSprite.getTexture() != &image)
		{
			m_videoSprite.setTexture(image);
		}
	}
}
