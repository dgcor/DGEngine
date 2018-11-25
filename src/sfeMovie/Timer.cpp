/*
 *  Timer.cpp
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

#include "Timer.hpp"

namespace sfe
{
	void Timer::addObserver(Observer& anObserver, int priority)
	{
		for (const auto& obs : m_observers)
		{
			if (obs.first == &anObserver)
			{
				return;
			}
		}
		for (auto it = m_observers.begin(); it != m_observers.end(); ++it)
		{
			if (priority < it->second)
			{
				m_observers.insert(it, std::make_pair(&anObserver, priority));
				return;
			}
		}
		m_observers.push_back(std::make_pair(&anObserver, priority));
	}

	void Timer::removeObserver(Observer& anObserver)
	{
		for (auto it = m_observers.begin(); it != m_observers.end(); ++it)
		{
			if (it->first == &anObserver)
			{
				m_observers.erase(it);
				break;
			}
		}
	}

	void Timer::play()
	{
		if (getStatus() == Playing)
		{
			return;
		}

		notifyObservers(Playing);

		Status oldStatus = getStatus();
		m_status = Playing;
		m_timer.restart();

		notifyObservers(oldStatus, getStatus());
	}

	void Timer::pause()
	{
		if (getStatus() == Paused)
		{
			return;
		}

		Status oldStatus = getStatus();
		m_status = Paused;

		if (oldStatus != Stopped)
		{
			m_pausedTime += m_timer.getElapsedTime();
		}

		notifyObservers(oldStatus, getStatus());
	}

	void Timer::stop()
	{
		if (getStatus() == Stopped)
		{
			return;
		}

		Status oldStatus = getStatus();
		m_status = Stopped;
		m_pausedTime = sf::Time::Zero;

		notifyObservers(oldStatus, getStatus());

		seek(sf::Time::Zero);
	}

	bool Timer::seek(sf::Time position)
	{
		Status oldStatus = getStatus();
		sf::Time oldPosition = getOffset();
		bool couldSeek = false;

		if (oldStatus == Playing)
		{
			pause();
		}

		m_pausedTime = position;
		couldSeek = notifyObservers(oldPosition);

		if (oldStatus == Playing)
		{
			play();
		}

		return couldSeek;
	}

	Status Timer::getStatus() const
	{
		return m_status;
	}

	sf::Time Timer::getOffset() const
	{
		if (Timer::getStatus() == Playing)
		{
			return m_pausedTime + m_timer.getElapsedTime();
		}
		return m_pausedTime;
	}

	void Timer::notifyObservers(Status futureStatus)
	{
		for (auto& observer : m_observers)
		{
			switch (futureStatus)
			{
			case Playing:
				observer.first->willPlay(*this);
				break;
			default:
				break;
			}
		}
	}

	void Timer::notifyObservers(Status oldStatus, Status newStatus)
	{
		if (oldStatus == newStatus)
		{
			return;
		}
		for (auto& observer : m_observers)
		{
			switch (newStatus)
			{
			case Playing:
				observer.first->didPlay(*this, oldStatus);
				break;
			case Paused:
				observer.first->didPause(*this, oldStatus);
				break;
			case Stopped:
				observer.first->didStop(*this, oldStatus);
				break;
			default:
				break;
			}
		}
	}

	bool Timer::notifyObservers(sf::Time oldPosition)
	{
		if (getStatus() == Playing)
		{
			return false;
		}
		bool successfullSeeking = true;
		for (auto& observer : m_observers)
		{
			if (observer.first->didSeek(*this, oldPosition) == false)
			{
				successfullSeeking = false;
			}
		}
		return successfullSeeking;
	}
}
