#pragma once

#include <memory>
#include <list>
#include <SFML/System/Time.hpp>
#include "Actions/Action.h"
#include "Event.h"

class EventManager
{
private:
	std::list<Event> events;

public:
	void addBack(const Event& event_) { events.push_back(event_); }
	void addBack(const std::shared_ptr<Action>& action) { events.push_back(Event(action)); }
	void addFront(const Event& event_) { events.push_front(event_); }
	void addFront(const std::shared_ptr<Action>& action) { events.push_front(Event(action)); }

	void remove(std::string& id)
	{
		if (id.empty())
		{
			return;
		}

		for (auto& evt : events)
		{
			if (evt.getId() == id)
			{
				evt = Event(nullptr);
			}
		}
	}

	void update(Game& game)
	{
		for (auto it = events.begin(); it != events.end();)
		{
			if (it->update(game) == true)
			{
				it = events.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
};
