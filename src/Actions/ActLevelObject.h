#pragma once

#include "Action.h"
#include "Game.h"
#include "Game/LevelObject.h"

class ActLevelObjDelete : public Action
{
private:
	std::string idLevel;
	std::string idObject;

public:
	ActLevelObjDelete(const std::string& idLevel_, const std::string& idObject_)
		: idLevel(idLevel_), idObject(idObject_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			level->deleteLevelObjectById(idObject);
		}
		return true;
	}
};

class ActLevelObjDeleteByClass : public Action
{
private:
	std::string idLevel;
	std::string idClass;

public:
	ActLevelObjDeleteByClass(const std::string& idLevel_, const std::string& idClass_)
		: idLevel(idLevel_), idClass(idClass_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			level->deleteLevelObjectByClass(idClass);
		}
		return true;
	}
};

class ActLevelObjExecuteAction : public Action
{
private:
	std::string idLevel;
	std::string idObject;

public:
	ActLevelObjExecuteAction(const std::string& idLevel_,
		const std::string& idObject_) : idLevel(idLevel_), idObject(idObject_) {}

	virtual bool execute(Game& game) noexcept
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto obj = level->getLevelObject(idObject);
			if (obj != nullptr)
			{
				obj->executeAction(game);
			}
		}
		return true;
	}
};

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
