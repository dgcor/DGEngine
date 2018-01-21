#pragma once

#include "ItemXY.h"
#include <string>
#include <variant>

class ItemCoordInventory
{
private:
	std::string playerId;
	int16_t inventoryIdx{ 0 };
	union
	{
		ItemXY itemXY;
		uint16_t itemIdx;
	};

public:
	// inventoryIdx = -1 points to player's selected item
	ItemCoordInventory() noexcept : inventoryIdx(-1), itemIdx(0) {};

	ItemCoordInventory(const std::string& playerId_) :
		playerId(playerId_), inventoryIdx(-1), itemIdx(0) {};

	ItemCoordInventory(const std::string& playerId_, size_t inventoryIdx_, size_t itemIdx_)
		: playerId(playerId_),
		inventoryIdx(((int16_t)inventoryIdx_) & 0x7FFF),
		itemIdx((int16_t)itemIdx_) {}

	ItemCoordInventory(const std::string& playerId_, size_t inventoryIdx_, const ItemXY& itemXY_)
		: playerId(playerId_),
		inventoryIdx(((int16_t)inventoryIdx_) | 0x8000),
		itemXY(itemXY_) {}

	bool isSelectedItem() const noexcept
	{
		return (inventoryIdx == -1);
	}

	bool isCoordXY() const noexcept
	{
		if (inventoryIdx == -1)
		{
			return false;
		}
		return (inventoryIdx & 0x8000) != 0;
	}

	size_t getInventoryIdx() const noexcept
	{
		if (isSelectedItem() == true)
		{
			return 0;
		}
		return (inventoryIdx & 0x7FFF);
	}

	size_t getItemIdx() const noexcept
	{
		if (isCoordXY() == true)
		{
			return 0;
		}
		return itemIdx;
	}

	ItemXY getItemXY() const noexcept
	{
		if (isCoordXY() == false)
		{
			return{};
		}
		return itemXY;
	}

	const std::string& getPlayerId() const noexcept { return playerId; }
};

typedef std::variant<MapCoord, ItemCoordInventory> ItemLocation;
