#pragma once

#include "Actions/Action.h"
#include <memory>
#include <vector>

class BaseClassActions
{
private:
	std::vector<std::pair<uint16_t, std::shared_ptr<Action>>> actions;

public:
	virtual ~BaseClassActions() = default;
	std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept;
	void setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action_);
	void executeAction(Game& game, uint16_t nameHash16, bool executeNow = false) const;
};
