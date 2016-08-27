#pragma once

#include "Actions/Action.h"
#include <memory>
#include <SFML/System/Time.hpp>
#include <string>

class Event
{
private:
	std::string id;
	std::shared_ptr<Action> action;
	sf::Time frameTime;
	sf::Time currentTime;

public:
	explicit Event(const std::shared_ptr<Action>& action_, const sf::Time& frameTime_ = sf::Time::Zero)
		: action(action_), frameTime(frameTime_) {}

	const std::string& getId() { return id; }

	void setAction(const std::shared_ptr<Action>& action_) { action = action_; }
	void setId(const std::string& id_) { id = id_; }

	bool update(Game& game);
};
