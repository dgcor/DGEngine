
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
#include "Macros.hpp"

namespace sfe
{
    Timer::Observer::Observer()
    {
    }
    
    Timer::Observer::~Observer()
    {
    }
    
    void Timer::Observer::willPlay(const Timer& timer)
    {
    }
    
    void Timer::Observer::didPlay(const Timer& timer, Status previousStatus)
    {
    }
    
    void Timer::Observer::didPause(const Timer& timer, Status previousStatus)
    {
    }
    
    void Timer::Observer::didStop(const Timer& timer, Status previousStatus)
    {
    }
    
    bool Timer::Observer::didSeek(const Timer& timer, sf::Time oldPosition)
    {
        return true;
    }
    
    Timer::Timer() :
    m_pausedTime(sf::Time::Zero),
    m_status(Stopped),
    m_timer(),
    m_observers()
    {
    }
    
    void Timer::addObserver(Observer& anObserver, int priority)
    {
        CHECK(m_observers.find(&anObserver) == m_observers.end(), "Timer::addObserver() - cannot add the same observer twice");
        
        m_observers.insert(std::make_pair(&anObserver, priority));
        m_observersByPriority[priority].insert(&anObserver);
    }
    
    void Timer::removeObserver(Observer& anObserver)
    {
        std::map<Observer*, int>::iterator it = m_observers.find(&anObserver);
        
        if (it != m_observers.end())
        {
            m_observersByPriority[it->second].erase(&anObserver);
            m_observers.erase(it);
        }
    }
    
    void Timer::play()
    {
        CHECK(getStatus() != Playing, "Timer::play() - timer playing twice");
        
        notifyObservers(Playing);
        
        Status oldStatus = getStatus();
        m_status = Playing;
        m_timer.restart();
        
        notifyObservers(oldStatus, getStatus());
    }
    
    void Timer::pause()
    {
        CHECK(getStatus() != Paused, "Timer::pause() - timer paused twice");
        
        Status oldStatus = getStatus();
        m_status = Paused;
        
        if (oldStatus != Stopped)
            m_pausedTime += m_timer.getElapsedTime();
        
        notifyObservers(oldStatus, getStatus());
    }
    
    void Timer::stop()
    {
        CHECK(getStatus() != Stopped, "Timer::stop() - timer stopped twice");
        
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
            pause();
        
        m_pausedTime = position;
        couldSeek = notifyObservers(oldPosition);
        
        if (oldStatus == Playing)
            play();
        
        return couldSeek;
    }
    
    Status Timer::getStatus() const
    {
        return m_status;
    }
    
    sf::Time Timer::getOffset() const
    {
        if (Timer::getStatus() == Playing)
            return m_pausedTime + m_timer.getElapsedTime();
        else
            return m_pausedTime;
    }
    
    void Timer::notifyObservers(Status futureStatus)
    {
        for (std::pair<int, std::set<Observer*> >&& pairByPriority : m_observersByPriority)
        {
            for (Observer* observer : pairByPriority.second)
            {
                switch(futureStatus)
                {
                    case Playing:
                        observer->willPlay(*this);
                        break;
                        
                    default:
                        CHECK(false, "Timer::notifyObservers() - unhandled case in switch");
                }
            }
        }
    }
    
    void Timer::notifyObservers(Status oldStatus, Status newStatus)
    {
        CHECK(oldStatus != newStatus, "Timer::notifyObservers() - inconsistency: no change happened");
        
        for (std::pair<int, std::set<Observer*> >&& pairByPriority : m_observersByPriority)
        {
            for (Observer* observer : pairByPriority.second)
            {
                switch(newStatus)
                {
                    case Playing:
                        observer->didPlay(*this, oldStatus);
                        break;
                        
                    case Paused:
                        observer->didPause(*this, oldStatus);
                        break;
                        
                    case Stopped:
                        observer->didStop(*this, oldStatus);
                        break;
                    default:
                        break;
                }
            }
        }
    }
    
    bool Timer::notifyObservers(sf::Time oldPosition)
    {
        CHECK(getStatus() != Playing, "inconsistency in timer");
        bool successfullSeeking = true;
        
        for (std::pair<int, std::set<Observer*> >&& pairByPriority : m_observersByPriority)
        {
            for (Observer* observer : pairByPriority.second)
            {
                if (! observer->didSeek(*this, oldPosition))
                    successfullSeeking = false;
            }
        }
        
        return successfullSeeking;
    }
    
}
