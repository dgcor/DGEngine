#pragma once

#include "Action.h"
#include "Game.h"
#include "Game/GameProperties.h"
#include "Game/Level.h"
#include <string>

class ActPlayerAddGold : public Action
{
private:
	std::string idPlayer;
	std::string idLevel;
	Variable gold;
	bool remove;

public:
	ActPlayerAddGold(const std::string& idPlayer_, const std::string& idLevel_,
		const Variable& gold_, bool remove_) : idPlayer(idPlayer_), idLevel(idLevel_),
		gold(gold_), remove(remove_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				LevelObjValue goldVal = 0;
				if (gold.is<std::string>() == true)
				{
					Variable prop2;
					if (game.getVarOrProp(gold.get<std::string>(), prop2) == true)
					{
						if (prop2.is<int64_t>() == true)
						{
							goldVal = (LevelObjValue)prop2.get<int64_t>();
						}
					}
				}
				else if (gold.is<int64_t>() == true)
				{
					goldVal = (LevelObjValue)gold.get<int64_t>();
				}
				if (goldVal != 0)
				{
					if (remove == true)
					{
						player->addGold(*level, -std::abs(goldVal));
					}
					else
					{
						player->addGold(*level, goldVal);
					}
				}
			}
		}
		return true;
	}
};

class ActPlayerMove : public Action
{
private:
	std::string idPlayer;
	std::string idLevel;
	MapCoord position;
	bool resetDirection;

public:
	ActPlayerMove(const std::string& idPlayer_, const std::string& idLevel_,
		const MapCoord& pos_, bool resetDirection_)
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
				player->clearWalkPath();
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
				MapCoord a = player->MapPosition();
				MapCoord b;
				auto hoverObj = level->getHoverObject();
				if (hoverObj != nullptr && hoverObj != player)
				{
					b = hoverObj->MapPosition();
				}
				else
				{
					b = level->getMapCoordOverMouse();
				}
				player->setWalkPath(level->Map().getPath(a, b));
			}
		}
		return true;
	}
};

class ActPlayerSetDirection : public Action
{
private:
	std::string idPlayer;
	std::string idLevel;
	PlayerDirection direction;

public:
	ActPlayerSetDirection(const std::string& idPlayer_,
		const std::string& idLevel_, PlayerDirection direction_)
		: idPlayer(idPlayer_), idLevel(idLevel_), direction(direction_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				player->setDirection(direction);
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

class ActPlayerSetSpeed : public Action
{
private:
	std::string idPlayer;
	std::string idLevel;
	int speed;

public:
	ActPlayerSetSpeed(const std::string& idPlayer_, const std::string& idLevel_,
		int speed_) : idPlayer(idPlayer_), idLevel(idLevel_), speed(speed_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				player->setWalkSpeed(speed);
			}
		}
		return true;
	}
};
