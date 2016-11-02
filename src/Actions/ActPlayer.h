#pragma once

#include "Action.h"
#include "Game.h"
#include "Game/Level.h"
#include "Game/PlayerProperties.h"
#include <string>

class ActPlayerMove : public Action
{
private:
	std::string idPlayer;
	std::string idLevel;
	MapCoord position;
	bool resetDirection;

public:
	ActPlayerMove(const std::string& idPlayer_, const std::string& idLevel_,
		const MapCoord pos_, bool resetDirection_)
		: idPlayer(idPlayer_), idLevel(idLevel_),
		position(pos_), resetDirection(resetDirection_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				player->MapPosition(*level, position);
				if (resetDirection == true)
				{
					player->setDirection(PlayerDirection::Front);
				}
			}
		}
		return true;
	}
};

class ActPlayerMoveToClick : public Action
{
private:
	std::string idPlayer;
	std::string idLevel;

public:
	ActPlayerMoveToClick(const std::string& idPlayer_, const std::string& idLevel_)
		: idPlayer(idPlayer_), idLevel(idLevel_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				auto a = player->MapPosition();
				auto b = level->getMapCoordOverMouse();
				player->setWalkPath(level->Map().getPath(a, b));
			}
		}
		return true;
	}
};

class ActPlayerSetPalette : public Action
{
private:
	std::string idPlayer;
	std::string idLevel;
	size_t palette;

public:
	ActPlayerSetPalette(const std::string& idPlayer_,
		const std::string& idLevel_, size_t palette_)
		: idPlayer(idPlayer_), idLevel(idLevel_), palette(palette_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				player->setPalette(palette);
			}
		}
		return true;
	}
};

class ActPlayerSetProperty : public Action
{
private:
	std::string idPlayer;
	std::string idLevel;
	std::string prop;
	Variable value;

public:
	ActPlayerSetProperty(const std::string& idPlayer_, const std::string& idLevel_,
		const std::string& prop_, const Variable& value_)
		: idPlayer(idPlayer_), idLevel(idLevel_), prop(prop_), value(value_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				Variable prop2(prop);
				game.getVarOrProp(prop, prop2);
				if (prop2.is<std::string>() == true)
				{
					const auto& propVal = prop2.get<std::string>();
					auto value2 = value;
					if (value2.is<std::string>() == true)
					{
						game.getVarOrProp(value2.get<std::string>(), value2);
					}
					player->setProperty(propVal, value2);
				}
			}
		}
		return true;
	}
};
