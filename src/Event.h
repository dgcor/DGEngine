#pragma once

#include "Actions/Action.h"
#include <memory>
#include <string>
#include <string_view>
#include "Utils/ElapsedTime.h"

class Event : public Action
{
private:
	std::string id;
	std::shared_ptr<Action> action;
	ElapsedTime elapsedTime;

public:
	explicit Event(const std::shared_ptr<Action>& action_,
		const sf::Time& timeout_ = sf::Time::Zero)
		: action(action_), elapsedTime(timeout_) {}

	const std::string& getId() const noexcept { return id; }
	void setId(const std::string_view id_) { id = id_; }

	void setAction(const std::shared_ptr<Action>& action_) noexcept { action = action_; }

	void resetTime() noexcept { elapsedTime.reset(); }

	bool execute(Game& game) override;
};
