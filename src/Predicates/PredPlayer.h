#pragma once

#include "Predicate.h"
#include "Game.h"

class PredPlayerCanEquip : public Predicate
{
private:
	std::string idLevel;
	std::string idPlayer;
	ItemLocation itemLocation;

public:
	PredPlayerCanEquip(const std::string& idLevel_,
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
					return { player->canEquipItem(*item) };
				}
			}
		}
		return { false };
	}
};

class PredPlayerGetMaxGoldCapacity : public Predicate
{
private:
	std::string idLevel;
	std::string idPlayer;

public:
	PredPlayerGetMaxGoldCapacity(const std::string& idLevel_,
		const std::string& idPlayer_)
		: idLevel(idLevel_), idPlayer(idPlayer_) {}

	virtual Variable getResult(const Game& game) const
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				return { (int64_t)player->getMaxGoldCapacity(*level) };
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
					return { player->hasItemSlot(*item) };
				}
			}
		}
		return { false };
	}
};
