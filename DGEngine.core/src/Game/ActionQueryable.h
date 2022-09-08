#pragma once

#include "Action.h"
#include <memory>

class ActionQueryable
{
public:
	virtual std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept = 0;

	virtual bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept = 0;
};
