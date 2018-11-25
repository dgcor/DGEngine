#pragma once

#include <memory>
#include <list>
#include "Actions/Action.h"
#include "Event.h"

class EventManager
{
private:
	std::list<std::shared_ptr<Action>> events;

public:
	void addBack(const std::shared_ptr<Action>& action) { events.push_back(action); }
	void addFront(const std::shared_ptr<Action>& action) { events.push_front(action); }

	bool exists(const std::string_view id) const
	{
		if (id.empty() == false)
		{
			for (const auto& evt : events)
			{
				auto actionEvt = dynamic_cast<Event*>(evt.get());
				if (actionEvt != nullptr)
				{
					if (actionEvt->getId() == id)
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	void remove(const std::string& id)
	{
		if (id.empty() == true)
		{
			return;
		}
		for (auto& evt : events)
		{
			auto actionEvt = dynamic_cast<Event*>(evt.get());
			if (actionEvt != nullptr)
			{
				if (actionEvt->getId() == id)
				{
					evt = nullptr;
				}
			}
		}
	}

	void resetTime(const std::string& id)
	{
		if (id.empty() == true)
		{
			return;
		}
		for (auto& evt : events)
		{
			auto actionEvt = dynamic_cast<Event*>(evt.get());
			if (actionEvt != nullptr)
			{
				if (actionEvt->getId() == id)
				{
					actionEvt->resetTime();
				}
			}
		}
	}

	void update(Game& game)
	{
		for (auto it = events.begin(); it != events.end();)
		{
			if ((*it) == nullptr ||
				(*it)->execute(game) == true)
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
