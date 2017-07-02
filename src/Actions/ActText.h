#pragma once

#include "Action.h"
#include "Game.h"
#include "Text2.h"

class ActTextSetColor : public Action
{
private:
	std::string id;
	sf::Color color;

public:
	ActTextSetColor(const std::string& id_, const sf::Color& color_) : id(id_), color(color_) {}

	virtual bool execute(Game& game)
	{
		auto text = game.Resources().getResource<Text2>(id);
		if (text != nullptr)
		{
			text->setColor(color);
		}
		return true;
	}
};
