#pragma once

#include "Action.h"
#include "Game.h"
#include <memory>
#include <string>

class ActActionSet : public Action
{
private:
	std::string key;
	std::shared_ptr<Action> action;

public:
	ActActionSet(const std::string& key_, const std::shared_ptr<Action>& action_)
		: key(key_), action(action_) {}

	bool execute(Game& game) override
	{
		if (key.empty() == false)
		{
			game.Resources().setAction(key, action);
		}
		return true;
	}
};
