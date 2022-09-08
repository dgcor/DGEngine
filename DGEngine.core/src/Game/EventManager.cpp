#include "EventManager.h"
#include "Event.h"

void EventManager::tryAddBack(const std::shared_ptr<Action>& action)
{
	if (action != nullptr)
	{
		events.push_back(action);
	}
}

void EventManager::tryAddFront(const std::shared_ptr<Action>& action)
{
	if (action != nullptr)
	{
		events.push_front(action);
	}
}

bool EventManager::exists(const std::string_view id) const
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

void EventManager::remove(const std::string& id)
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

void EventManager::removeAll()
{
	for (auto& evt : events)
	{
		if (dynamic_cast<Event*>(evt.get()) != nullptr)
		{
			evt = nullptr;
		}
	}
}

void EventManager::resetTime(const std::string& id)
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

void EventManager::update(Game& game)
{
	for (auto it = events.begin(); it != events.end();)
	{
		auto evt = it->get();
		if (evt == nullptr ||
			evt->execute(game) == true ||
			dynamic_cast<Event*>(evt) == nullptr)
		{
			it = events.erase(it);
		}
		else
		{
			++it;
		}
	}
}
