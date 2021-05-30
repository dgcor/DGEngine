#pragma once

#include "Action.h"
#include "Game.h"

class ActionList : public Action
{
private:
	std::vector<std::shared_ptr<Action>> actions;

public:
	void add(const std::shared_ptr<Action>& action) { actions.push_back(action); }

	bool execute(Game& game) override
	{
		for (auto& action : actions)
		{
			if (action != nullptr)
			{
				action->execute(game);
			}
		}
		return true;
	}
};
