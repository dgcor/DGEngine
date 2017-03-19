#pragma once

#include "Action.h"
#include "Game.h"
#include "Parser/Parser.h"
#include <string>

class ActEventAdd : public Action
{
private:
	std::string id;
	std::shared_ptr<Action> action;
	sf::Time seconds;
	bool addToFront;

public:
	ActEventAdd(const std::string& id_, const std::shared_ptr<Action>& action_,
		const sf::Time& seconds_, bool addToFront_) : id(id_), action(action_),
		seconds(seconds_), addToFront(addToFront_) {}

	virtual bool execute(Game& game)
	{
		if (action != nullptr)
		{
			Event ev(action, seconds);
			ev.setId(id);
			if (addToFront == true)
			{
				game.Events().addFront(ev);
			}
			else
			{
				game.Events().addBack(ev);
			}
		}
		return true;
	}
};

class ActEventDelete : public Action
{
private:
	std::string id;

public:
	ActEventDelete(const std::string& id_) : id(id_) {}

	virtual bool execute(Game& game)
	{
		game.Events().remove(id);
		return true;
	}
};

class ActEventResetTime : public Action
{
private:
	std::string id;

public:
	ActEventResetTime(const std::string& id_) : id(id_) {}

	virtual bool execute(Game& game)
	{
		game.Events().resetTime(id);
		return true;
	}
};
