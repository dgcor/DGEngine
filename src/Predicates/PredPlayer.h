#pragma once

#include "Predicate.h"
#include "Game.h"

class PredPlayerCanUse : public Predicate
{
private:
	std::string idLevel;
	std::string idPlayer;
	ItemLocation itemLocation;

public:
	PredPlayerCanUse(const std::string& idLevel_,
		const std::string& idPlayer_, const ItemLocation& itemLocation_)
		: idLevel(idLevel_), idPlayer(idPlayer_), itemLocation(itemLocation_) {}

	virtual Variable getResult(const Game& game) const
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto item = level->getItem(itemLocation);
			if (item != nullptr)
			{
				auto player = level->getPlayerOrCurrent(idPlayer);
				if (player != nullptr)
				{
					return { player->canUseItem(*item) };
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

	virtual Variable getResult(const Game& game) const
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

	virtual Variable getResult(const Game& game) const
	{
		auto level = game.Resources().getLevel(idLevel);
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
