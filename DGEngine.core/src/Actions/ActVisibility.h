#pragma once

#include "Action.h"
#include "Game.h"

class ActToggleVisible : public Action
{
private:
	std::string id;

public:
	ActToggleVisible(const std::string_view id_) : id(id_) {}

	bool execute(Game& game) override
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
	ActSetVisible(const std::string_view id_, bool visible_) : id(id_), visible(visible_) {}

	bool execute(Game& game) override
	{
		auto item = game.Resources().getDrawable(id);
		if (item != nullptr)
		{
			item->Visible(visible);
		}
		return true;
	}
};
