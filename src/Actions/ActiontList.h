#pragma once

#include "Action.h"
#include "Game.h"
#include <string>

class ActionList : public Action
{
private:
	std::vector<std::shared_ptr<Action>> actions;

public:
	void add(const std::shared_ptr<Action>& action) { actions.push_back(action); }

	virtual bool execute(Game& game)
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
