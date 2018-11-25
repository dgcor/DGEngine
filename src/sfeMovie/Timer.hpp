/*
 *  Timer.hpp
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

#include "Movie.hpp"
#include <SFML/System.hpp>

namespace sfe
{
	class Timer
	{
	public:
		class Observer
		{
		public:
			/** Default destructor
			 */
			virtual ~Observer() = default;

			/** Called by @a timer before playing if this Observer is registered for notifications
			 *
			 * Playing won't start until all Observers are done executing willPlay()
			 *
			 * @param timer the timer that generated the notification
			 */
			virtual void willPlay(const Timer& timer) {};

			/** Called by @a timer when playing if this Observer is registered for notifications
			 *
			 * @param timer the timer that generated the notification
			 * @param previousStatus the timer's status before playing
			 */
			virtual void didPlay(const Timer& timer, Status previousStatus) {};

			/** Called by @a timer when pausing if this Observer is registered for notifications
			 *
			 * @param timer the timer that generated the notification
			 * @param previousStatus the timer's status before playing
			 */
			virtual void didPause(const Timer& timer, Status previousStatus) {};

			/** Called by @a timer when stopping if this Observer is registered for notifications
			 *
			 * @param timer the timer that generated the notification
			 * @param previousStatus the timer's status before playing
			 */
			virtual void didStop(const Timer& timer, Status previousStatus) {};

			/** Called by @a timer right after seeking if this Observer is registered for notifications
			 *
			 * When this method is called, the timer is guaranteed to be paused or stopped
			 *
			 * @param timer the timer that generated the notification
			 * @param position the position before seeking
			 * @return true if the observer successfully responded to the seeking event
			 */
			virtual bool didSeek(const Timer& timer, sf::Time oldPosition) { return true; };
		};

		/** Default constructor
		 */
		Timer() = default;

		/** Register an observer that should be notified when this timer is
		 * played, paused or stopped
		 *
		 * @param anObserver the observer that should receive notifications
		 * @param priority the priority that should be taken into account when distributing notifications,
		 * observers with the lowest priority value will be notified first
		 */
		void addObserver(Observer& anObserver, int priority = 0);

		/** Stop sending notifications to this observer
		 *
		 * @param anObserver the observer that must receive no more notification
		 */
		void removeObserver(Observer& anObserver);

		/** Start this timer and notify all observers
		 */
		void play();

		/** Pause this timer (but do not reset it) and notify all observers
		 */
		void pause();

		/** Stop this timer and reset it and notify all observers
		 */
		void stop();

		/** Seek to the given position, the timer's offset is updated accordingly
		 *
		 * If the timer was playing, it is paused, seeking occurs, then it is resumed.
		 * The timer offset is always updated, even if seeking fails
		 *
		 * @param position the new wished timer position
		 * @return true if seeking succeeded, false otherwise
		 */
		bool seek(sf::Time position);

		/** Return this timer status
		 *
		 * @return Playing, Paused or Stopped
		 */
		Status getStatus() const;

		/** Return the timer's time
		 *
		 * @return the timer's time
		 */
		sf::Time getOffset() const;

	private:
		/** Notify all observers that the timer's status is about to change to @a futureStatus
		 *
		 * The status change won't occur before all observers have received the noficiation
		 *
		 * @param futureStatus the status to which this timer is about to change
		 */
		void notifyObservers(Status newStatus);

		/** Notify all observers that the timer's status changed from @a oldStatus to @a newStatus
		 *
		 * @param oldStatus the timer's status before the state change
		 * @param newStatus the timer's status after the state change
		 */
		void notifyObservers(Status oldStatus, Status newStatus);

		/** Notify all observers that the timer is seeking to a new position
		 *
		 * When the observer receives the notification, the timer is guaranteed to be paused or stopped
		 *
		 * @param oldPosition the timer position before seeking
		 * @return true if all the observers successfully responsed to the seek event
		 */
		bool notifyObservers(sf::Time oldPosition);

		sf::Time m_pausedTime{ sf::Time::Zero };
		Status m_status{ Stopped };
		sf::Clock m_timer;
		std::vector<std::pair<Observer*, int>> m_observers;
	};
}
