#pragma once

#include "Action.h"
#include <list>
#include <memory>
#include <string_view>

class EventManager
{
private:
	std::list<std::shared_ptr<Action>> events;

public:
	void addBack(const std::shared_ptr<Action>& action) { events.push_back(action); }
	void addFront(const std::shared_ptr<Action>& action) { events.push_front(action); }

	void tryAddBack(const std::shared_ptr<Action>& action);
	void tryAddFront(const std::shared_ptr<Action>& action);

	bool exists(const std::string_view id) const;

	void remove(const std::string& id);

	void removeAll();

	void resetTime(const std::string& id);

	void update(Game& game);
};
