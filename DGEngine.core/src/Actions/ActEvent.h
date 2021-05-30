#pragma once

#include "Action.h"
#include "Game.h"
#include "Parser/Parser.h"

class ActEventAdd : public Action
{
private:
	std::string id;
	std::shared_ptr<Action> action;
	sf::Time time;
	bool addToFront;

public:
	ActEventAdd(const std::string_view id_, const std::shared_ptr<Action>& action_,
		sf::Time time_, bool addToFront_) : id(id_), action(action_),
		time(time_), addToFront(addToFront_) {}

	bool execute(Game& game) override
	{
		if (action != nullptr)
		{
			auto ev = std::make_shared<Event>(action, time);
			ev->setId(id);
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
	ActEventDelete(const std::string_view id_) : id(id_) {}

	bool execute(Game& game) override
	{
		game.Events().remove(id);
		return true;
	}
};

class ActEventDeleteAll : public Action
{
public:
	ActEventDeleteAll() {}

	bool execute(Game& game) override
	{
		game.Events().removeAll();
		return true;
	}
};

class ActEventResetTime : public Action
{
private:
	std::string id;

public:
	ActEventResetTime(const std::string_view id_) : id(id_) {}

	bool execute(Game& game) override
	{
		game.Events().resetTime(id);
		return true;
	}
};
