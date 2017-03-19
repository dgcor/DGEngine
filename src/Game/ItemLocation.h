#pragma once

#include "ItemXY.h"
#include "Variant.h"

class ItemCoordInventory
{
private:
	union ItemXYOrIndex {
		ItemXY itemXY;
		uint16_t itemIdx;

		ItemXYOrIndex() : ItemXYOrIndex(0) {}
		ItemXYOrIndex(ItemXY itemXY_) : itemXY(itemXY_) {}
		ItemXYOrIndex(uint16_t itemIdx_) : itemIdx(itemIdx_) {}
	};

	std::string playerId;
	int16_t inventoryIdx{ 0 };
	ItemXYOrIndex itemPos;

public:
	// inventoryIdx = -1 points to player's selected item
	ItemCoordInventory() : inventoryIdx(-1) {};

	ItemCoordInventory(const std::string& playerId_) : playerId(playerId_), inventoryIdx(-1) {};

	ItemCoordInventory(const std::string& playerId_, size_t inventoryIdx_, size_t itemIdx_)
		: playerId(playerId_),
		inventoryIdx(((int16_t)inventoryIdx_) & 0x7FFF),
		itemPos((uint16_t)itemIdx_) {}

	ItemCoordInventory(const std::string& playerId_, size_t inventoryIdx_, const ItemXY& itemXY_)
		: playerId(playerId_),
		inventoryIdx(((int16_t)inventoryIdx_) | 0x8000),
		itemPos(itemXY_) {}

	bool isSelectedItem() const
	{
		return (inventoryIdx == -1);
	}

	bool isCoordXY() const
	{
		if (inventoryIdx == -1)
		{
			return false;
		}
		return (inventoryIdx & 0x8000) != 0;
	}

	size_t getInventoryIdx() const
	{
		if (isSelectedItem() == true)
		{
			return 0;
		}
		return (inventoryIdx & 0x7FFF);
	}

	size_t getItemIdx() const
	{
		if (isCoordXY() == true)
		{
			return 0;
		}
		return itemPos.itemIdx;
	}

	ItemXY getItemXY() const
	{
		if (isCoordXY() == false)
		{
			return{};
		}
		return itemPos.itemXY;
	}

	const std::string& getPlayerId() const { return playerId; }
};

typedef mapbox::util::variant<MapCoord, ItemCoordInventory> ItemLocation;
