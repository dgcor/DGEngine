#pragma once

#include "Action.h"
#include "Game.h"
#include "Utils/Utils.h"

class ActRandomList : public Action
{
private:
	std::vector<std::shared_ptr<Action>> actions;

public:
	void add(const std::shared_ptr<Action>& action) { actions.push_back(action); }

	bool execute(Game& game) override
	{
		if (actions.empty() == false)
		{
			auto idx = Utils::Random::get<size_t>(actions.size() - 1);
			if (actions[idx] != nullptr)
			{
				actions[idx]->execute(game);
			}
		}
		return true;
	}
};

class ActRandom : public Action
{
private:
	float percentage;
	std::shared_ptr<Action> action1;
	std::shared_ptr<Action> action2;

public:
	ActRandom(float percentage_, const std::shared_ptr<Action>& action1_,
		const std::shared_ptr<Action>& action2_) noexcept
		: percentage(percentage_), action1(action1_), action2(action2_) {}

	bool execute(Game& game) override
	{
		if (Utils::Random::getf<float>() < percentage)
		{
			if (action1 != nullptr)
			{
				action1->execute(game);
			}
		}
		else
		{
			if (action2 != nullptr)
			{
				action2->execute(game);
			}
		}
		return true;
	}
};
