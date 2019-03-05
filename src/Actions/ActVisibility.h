#pragma once

#include "Action.h"
#include "Game.h"

class ActToggleVisible : public Action
{
private:
	std::string id;

public:
	ActToggleVisible(const std::string& id_) : id(id_) {}

	virtual bool execute(Game& game)
	{
		auto item = game.Resources().getDrawable(id);
		if (item != nullptr)
		{
			item->Visible(!item->Visible());
		}
		return true;
	}
};

class ActSetVisible : public Action
{
private:
	std::string id;
	bool visible;

public:
	ActSetVisible(const std::string& id_, bool visible_) : id(id_), visible(visible_) {}

	virtual bool execute(Game& game)
	{
		auto item = game.Resources().getDrawable(id);
		if (item != nullptr)
		{
			item->Visible(visible);
		}
		return true;
	}
};
