#pragma once

#include "Action.h"
#include "Game.h"
#include "Game/LevelObject.h"

class ActLevelObjSetOutline : public Action
{
private:
	std::string idLevel;
	sf::Color outline;
	sf::Color ignore;

public:
	ActLevelObjSetOutline(const std::string& idLevel_,
		const sf::Color& outline_, const sf::Color& ignore_)
		: idLevel(idLevel_), outline(outline_), ignore(ignore_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto item = level->getHoverObject();
			if (item != nullptr)
			{
				item->setOutline(outline, ignore);
			}
		}
		return true;
	}
};

class ActLevelObjSetPalette : public Action
{
private:
	std::string idLevel;
	std::string idPalette;
	sf::Color color;

public:
	ActLevelObjSetPalette(const std::string& idLevel_,
		const std::string& idPalette_, const sf::Color& color_)
		: idLevel(idLevel_), idPalette(idPalette_), color(color_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto item = level->getHoverObject();
			if (item != nullptr)
			{
				if (item->hasPalette() == true)
				{
					auto palette = game.Resources().getPalette(idPalette);
					if (palette != nullptr)
					{
						item->setPalette(palette);
					}
				}
				else
				{
					item->setColor(color);
				}
			}
		}
		return true;
	}
};
