#pragma once

#include "Action.h"
#include "Game.h"
#include "Game/GameProperties.h"
#include "Game/Level.h"
#include "Game/Player.h"
#include <string>

class ActPlayerAddItemQuantity : public Action
{
private:
	std::string idPlayer;
	std::string idLevel;
	std::string itemClass;
	InventoryPosition invPos{ InventoryPosition::TopLeft };
	Variable quantity;
	bool remove;

public:
	ActPlayerAddItemQuantity(const std::string& idPlayer_, const std::string& idLevel_,
		const std::string& itemClass_, InventoryPosition invPos_, const Variable& quantity_,
		bool remove_) : idPlayer(idPlayer_), idLevel(idLevel_), itemClass(itemClass_),
		invPos(invPos_), quantity(quantity_), remove(remove_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto itemClassPtr = level->getClass<ItemClass>(itemClass);
			if (itemClassPtr != nullptr)
			{
				auto player = level->getPlayerOrCurrent(idPlayer);
				if (player != nullptr)
				{
					auto quantVal = (LevelObjValue)game.getVarOrPropLongV(quantity);
					if (quantVal != 0)
					{
						if (remove == true)
						{
							player->addItemQuantity(*itemClassPtr, -std::abs(quantVal), invPos);
						}
						else
						{
							player->addItemQuantity(*itemClassPtr, quantVal, invPos);
						}
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
				auto propVal = game.getVarOrPropStringS(prop);
				if (propVal.empty() == false)
				{
					auto value2 = game.getVarOrProp(value);
					Number32 currVal;
					if (player->getNumberProp(propVal.c_str(), currVal) == true)
					{
						if (std::holds_alternative<int64_t>(value2) == true)
						{
							if (remove == true)
							{
								currVal -= std::get<int64_t>(value2);
							}
							else
							{
								currVal += std::get<int64_t>(value2);
							}
						}
						else if (std::holds_alternative<double>(value2) == true)
						{
							if (remove == true)
							{
								currVal -= std::get<double>(value2);
							}
							else
							{
								currVal += std::get<double>(value2);
							}
						}
						else
						{
							return true;
						}
						if (player->setNumber(propVal, currVal, level) == true)
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
	PairFloat position;
	bool resetDirection;
	bool smooth;

public:
	ActPlayerMove(const std::string& idPlayer_, const std::string& idLevel_,
		const PairFloat& pos_, bool resetDirection_, bool smooth_)
		: idPlayer(idPlayer_), idLevel(idLevel_), position(pos_),
		resetDirection(resetDirection_), smooth(smooth_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				player->move(*level, position);
				level->updateCurrentMapViewCenter(smooth);
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
				PairFloat a = player->MapPosition();
				PairFloat b;
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

class ActPlayerSelectSpell : public Action
{
private:
	std::string idPlayer;
	std::string idLevel;
	std::string spell;

public:
	ActPlayerSelectSpell(const std::string& idPlayer_,
		const std::string& idLevel_, const std::string& spell_)
		: idPlayer(idPlayer_), idLevel(idLevel_), spell(spell_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				player->SelectedSpell(spell);
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
				auto prop2 = game.getVarOrPropStringS(prop);
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
				player->updateTexture();
			}
		}
		return true;
	}
};
