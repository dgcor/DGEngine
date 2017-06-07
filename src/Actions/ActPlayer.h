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
				auto goldVal = (LevelObjValue)game.getVarOrPropLong(gold);
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

class ActPlayerAddToProperty : public Action
{
private:
	std::string idPlayer;
	std::string idLevel;
	std::string prop;
	Variable value;
	bool remove;

public:
	ActPlayerAddToProperty(const std::string& idPlayer_, const std::string& idLevel_,
		const std::string& prop_, const Variable& value_, bool remove_) : idPlayer(idPlayer_),
		idLevel(idLevel_), prop(prop_), value(value_), remove(remove_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				auto propVal = game.getVarOrPropString(prop);
				if (propVal.empty() == false)
				{
					auto value2 = game.getVarOrProp(value);
					Number32 currVal;
					if (player->getNumberProp(propVal.c_str(), currVal) == true)
					{
						if (value2.is<int64_t>() == true)
						{
							if (remove == true)
							{
								currVal -= value2.get<int64_t>();
							}
							else
							{
								currVal += value2.get<int64_t>();
							}
						}
						else if (value2.is<double>() == true)
						{
							if (remove == true)
							{
								currVal -= value2.get<double>();
							}
							else
							{
								currVal += value2.get<double>();
							}
						}
						else
						{
							return true;
						}
						if (player->setNumber(propVal, currVal) == true)
						{
							player->updateProperties();
						}
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
				player->move(*level, position);
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

class ActPlayerSetDefaultSpeed : public Action
{
private:
	std::string idPlayer;
	std::string idLevel;
	AnimationSpeed speed;

public:
	ActPlayerSetDefaultSpeed(const std::string& idPlayer_,
		const std::string& idLevel_, const AnimationSpeed& speed_)
		: idPlayer(idPlayer_), idLevel(idLevel_), speed(speed_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				player->setDefaultSpeed(speed);
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
				auto prop2 = game.getVarOrPropString(prop);
				if (prop2.empty() == false)
				{
					auto value2 = game.getVarOrProp(value);
					player->setProperty(prop2, value2);
				}
			}
		}
		return true;
	}
};

class ActPlayerSetRestStatus : public Action
{
private:
	std::string idPlayer;
	std::string idLevel;
	uint8_t status;

public:
	ActPlayerSetRestStatus(const std::string& idPlayer_, const std::string& idLevel_,
		uint8_t status_) : idPlayer(idPlayer_), idLevel(idLevel_), status(status_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				player->setRestStatus(status);
				player->updateTexture();
			}
		}
		return true;
	}
};

class ActPlayerSetTextureIndex : public Action
{
private:
	std::string idPlayer;
	std::string idLevel;
	size_t idx;

public:
	ActPlayerSetTextureIndex(const std::string& idPlayer_,
		const std::string& idLevel_, size_t idx_)
		: idPlayer(idPlayer_), idLevel(idLevel_), idx(idx_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				player->setTextureIdx(idx);
			}
		}
		return true;
	}
};
