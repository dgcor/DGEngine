#pragma once

#include "Game/Action.h"
#include "Game/Drawables/Scrollable.h"
#include "Game/Game.h"

class ActScrollableSetSpeed : public Action
{
private:
	std::string id;
	sf::Time speed;

public:
	ActScrollableSetSpeed(const std::string_view id_, sf::Time speed_)
		: id(id_), speed(speed_) {}

	bool execute(Game& game) override
	{
		auto obj = game.Resources().getDrawable<Scrollable>(id);
		if (obj != nullptr)
		{
			obj->setSpeed(speed);
		}
		return true;
	}
};
