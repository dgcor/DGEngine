#pragma once

#include "Game/Action.h"
#include "Game/Game.h"
#include "Game/Level/Level.h"
#include "Game/Player/Player.h"

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
	ActPlayerAddItemQuantity(const std::string_view idPlayer_, const std::string_view idLevel_,
		const std::string_view itemClass_, InventoryPosition invPos_, const Variable& quantity_,
		bool remove_) : idPlayer(idPlayer_), idLevel(idLevel_), itemClass(itemClass_),
		invPos(invPos_), quantity(quantity_), remove(remove_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
		if (level != nullptr)
		{
			auto itemClassPtr = level->LevelObjects().getClass<ItemClass>(itemClass);
			if (itemClassPtr != nullptr)
			{
				auto player = level->getPlayerOrCurrent(idPlayer);
				if (player != nullptr)
				{
					auto quantVal = (LevelObjValue)game.getVarOrPropInt64V(quantity);
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
	ActPlayerAddToProperty(const std::string_view idPlayer_, const std::string_view idLevel_,
		const std::string_view prop_, const Variable& value_, bool remove_) : idPlayer(idPlayer_),
		idLevel(idLevel_), prop(prop_), value(value_), remove(remove_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
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
					if (player->getNumber(propVal.c_str(), currVal) == true)
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
						if (player->setNumber(propVal, currVal, level.get()) == true)
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
	ActPlayerMove(const std::string_view idPlayer_, const std::string_view idLevel_,
		const PairFloat& pos_, bool resetDirection_, bool smooth_)
		: idPlayer(idPlayer_), idLevel(idLevel_), position(pos_),
		resetDirection(resetDirection_), smooth(smooth_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				player->move(level->Map(), position);
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

class ActPlayerSelectSpell : public Action
{
private:
	std::string idPlayer;
	std::string idLevel;
	std::string spell;

public:
	ActPlayerSelectSpell(const std::string_view idPlayer_,
		const std::string_view idLevel_, const std::string_view spell_)
		: idPlayer(idPlayer_), idLevel(idLevel_), spell(spell_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
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
	ActPlayerSetDefaultSpeed(const std::string_view idPlayer_,
		const std::string_view idLevel_, const AnimationSpeed& speed_)
		: idPlayer(idPlayer_), idLevel(idLevel_), speed(speed_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
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
	ActPlayerSetDirection(const std::string_view idPlayer_,
		const std::string_view idLevel_, PlayerDirection direction_)
		: idPlayer(idPlayer_), idLevel(idLevel_), direction(direction_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
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
	ActPlayerSetProperty(const std::string_view idPlayer_, const std::string_view idLevel_,
		const std::string_view prop_, const Variable& value_)
		: idPlayer(idPlayer_), idLevel(idLevel_), prop(prop_), value(value_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
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
	ActPlayerSetRestStatus(const std::string_view idPlayer_, const std::string_view idLevel_,
		uint8_t status_) : idPlayer(idPlayer_), idLevel(idLevel_), status(status_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
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
	ActPlayerSetTextureIndex(const std::string_view idPlayer_,
		const std::string_view idLevel_, size_t idx_)
		: idPlayer(idPlayer_), idLevel(idLevel_), idx(idx_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
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

class ActPlayerWalk : public Action
{
private:
	std::string idPlayer;
	std::string idLevel;
	PlayerDirection direction;
	bool executeAction;

public:
	ActPlayerWalk(const std::string_view idPlayer_, const std::string_view idLevel_,
		PlayerDirection direction_, bool executeAction_) : idPlayer(idPlayer_),
		idLevel(idLevel_), direction(direction_), executeAction(executeAction_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				player->Walk(level->Map(), direction, executeAction);
			}
		}
		return true;
	}
};

class ActPlayerWalkToClick : public Action
{
private:
	std::string idPlayer;
	std::string idLevel;
	bool executeAction;

public:
	ActPlayerWalkToClick(const std::string_view idPlayer_, const std::string_view idLevel_,
		bool executeAction_) : idPlayer(idPlayer_), idLevel(idLevel_),
		executeAction(executeAction_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				PairFloat mapPos;
				auto hoverObj = level->LevelObjects().HoverObject();
				if (hoverObj != nullptr && hoverObj != player)
				{
					mapPos = hoverObj->MapPosition();
				}
				else
				{
					mapPos = level->getMapCoordOverMouse();
				}
				player->Walk(level->Map(), mapPos, executeAction);
			}
		}
		return true;
	}
};
