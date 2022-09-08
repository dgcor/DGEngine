#pragma once

#include "Game/Game.h"
#include "Game/Level/Level.h"
#include "Game/Player/Player.h"
#include "Game/Predicate.h"

class PredPlayerCanUseItem : public Predicate
{
private:
	std::string idLevel;
	std::string idPlayer;
	ItemLocation itemLocation;

public:
	PredPlayerCanUseItem(const std::string& idLevel_,
		const std::string& idPlayer_, const ItemLocation& itemLocation_)
		: idLevel(idLevel_), idPlayer(idPlayer_), itemLocation(itemLocation_) {}

	Variable getResult(const Game& game) const override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
		if (level != nullptr)
		{
			auto item = level->getItem(itemLocation);
			if (item != nullptr)
			{
				auto player = level->getPlayerOrCurrent(idPlayer);
				if (player != nullptr)
				{
					return { player->canUseObject(*item) };
				}
			}
		}
		return { false };
	}
};

class PredPlayerCanUseSpell : public Predicate
{
private:
	std::string idLevel;
	std::string idPlayer;
	std::string idSpell;

public:
	PredPlayerCanUseSpell(const std::string& idLevel_,
		const std::string& idPlayer_, const std::string& idSpell_)
		: idLevel(idLevel_), idPlayer(idPlayer_), idSpell(idSpell_) {}

	Variable getResult(const Game& game) const override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				auto spell = player->getSpell(idSpell);
				if (spell != nullptr)
				{
					return { player->canUseObject(*spell) };
				}
			}
		}
		return { false };
	}
};

class PredPlayerGetMaxItemCapacity : public Predicate
{
private:
	std::string idLevel;
	std::string idPlayer;
	std::string itemClass;

public:
	PredPlayerGetMaxItemCapacity(const std::string& idLevel_,
		const std::string& idPlayer_, const std::string& itemClass_)
		: idLevel(idLevel_), idPlayer(idPlayer_), itemClass(itemClass_) {}

	Variable getResult(const Game& game) const override
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
					return { (int64_t)player->getMaxItemCapacity(*itemClassPtr) };
				}
			}
		}
		return { (int64_t)0 };
	}
};

class PredPlayerHasFreeItemSlot : public Predicate
{
private:
	std::string idLevel;
	std::string idPlayer;
	ItemCoordInventory itemCoord;

public:
	PredPlayerHasFreeItemSlot(const std::string& idLevel_,
		const std::string& idPlayer_, const ItemCoordInventory& itemCoord_)
		: idLevel(idLevel_), idPlayer(idPlayer_), itemCoord(itemCoord_) {}

	Variable getResult(const Game& game) const override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
		if (level != nullptr)
		{
			auto item = level->getItem(itemCoord);
			if (item != nullptr)
			{
				auto player = level->getPlayerOrCurrent(idPlayer);
				if (player != nullptr)
				{
					return { player->hasFreeItemSlot(*item) };
				}
			}
		}
		return { false };
	}
};
