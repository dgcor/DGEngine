#pragma once

#include "Action.h"
#include "Game.h"
#include "Scrollable.h"

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
