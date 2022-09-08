#pragma once

#include "Game/Action.h"
#include "Game/Game.h"
#include "Game/Level/Level.h"
#include "Game/LevelObject/LevelObject.h"

class ActLevelObjDelete : public Action
{
private:
	std::string idLevel;
	std::string idObject;

public:
	ActLevelObjDelete(const std::string_view idLevel_, const std::string_view idObject_)
		: idLevel(idLevel_), idObject(idObject_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
		if (level != nullptr)
		{
			level->LevelObjects().deleteById(level->Map(), idObject);
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
	ActLevelObjDeleteByClass(const std::string_view idLevel_, const std::string_view idClass_)
		: idLevel(idLevel_), idClass(idClass_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
		if (level != nullptr)
		{
			level->LevelObjects().deleteByClass(level->Map(), idClass);
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
	ActLevelObjExecuteAction(const std::string_view idLevel_,
		const std::string_view idObject_) : idLevel(idLevel_), idObject(idObject_) {}

	bool execute(Game& game) noexcept override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
		if (level != nullptr)
		{
			auto obj = level->LevelObjects().get(idObject);
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
	ActLevelObjSetOutline(const std::string_view idLevel_,
		const sf::Color& outline_, const sf::Color& ignore_)
		: idLevel(idLevel_), outline(outline_), ignore(ignore_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
		if (level != nullptr)
		{
			auto item = level->LevelObjects().HoverObject();
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
	ActLevelObjSetPalette(const std::string_view idLevel_,
		const std::string_view idPalette_, const sf::Color& color_)
		: idLevel(idLevel_), idPalette(idPalette_), color(color_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
		if (level != nullptr)
		{
			auto item = level->LevelObjects().HoverObject();
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
