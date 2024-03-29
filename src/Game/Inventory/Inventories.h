#pragma once

#include <array>
#include "Game/GameHashes.h"
#include "Game/Utils/GameUtils2.h"
#include "Inventory.h"
#include "Utils/Utils.h"

template <size_t Size_>
class Inventories : private std::array<Inventory, Size_>
{
private:
	using InvArray = std::array<Inventory, Size_>;

public:
	using InvArray::begin;
	using InvArray::end;
	using InvArray::cbegin;
	using InvArray::cend;
	using InvArray::rbegin;
	using InvArray::rend;
	using InvArray::crbegin;
	using InvArray::crend;

	using InvArray::size;
	using InvArray::max_size;
	using InvArray::operator[];

	bool empty() const noexcept
	{
		for (const auto& inv : (*this))
		{
			if (inv.empty() == false)
			{
				return false;
			}
		}
		return true;
	}

	bool getFreeSlot(const Item& item, size_t& invIdx, size_t& itemIdx, InventoryPosition invPos) const
	{
		for (; invIdx < size(); invIdx++)
		{
			if ((*this)[invIdx].getFreeSlot(item, itemIdx, invPos) == true)
			{
				return true;
			}
		}
		return false;
	}

	bool hasFreeSlot(const Item& item) const
	{
		for (const auto& inv : (*this))
		{
			if (inv.hasFreeSlot(item) == true)
			{
				return true;
			}
		}
		return false;
	}

	bool hasItem(uint16_t classIdHash16) const
	{
		for (const auto& inv : (*this))
		{
			if (inv.hasItem(classIdHash16) == true)
			{
				return true;
			}
		}
		return false;
	}

	unsigned countFreeSlots(uint16_t classIdHash16) const
	{
		unsigned count = 0;
		for (const auto& inv : (*this))
		{
			count += inv.countFreeSlots(classIdHash16);
		}
		return count;
	}

	bool findByClass(uint16_t classIdHash16, size_t& invIdx, size_t& itemIdx, Item*& item) const
	{
		for (; invIdx < size(); invIdx++)
		{
			if ((*this)[invIdx].findByClass(classIdHash16, itemIdx, item) == true)
			{
				return true;
			}
			else if (invIdx < size() - 1)
			{
				itemIdx = 0;
			}
		}
		return false;
	}

	uint32_t findFreeQuantity(uint16_t classIdHash16, size_t& invIdx, size_t& itemIdx, Item*& item) const
	{
		for (; invIdx < size(); invIdx++)
		{
			uint32_t freeQuantity = 0;
			if ((freeQuantity = (*this)[invIdx].findFreeQuantity(
				classIdHash16, itemIdx, item)) > 0)
			{
				return freeQuantity;
			}
			else if (invIdx < size() - 1)
			{
				itemIdx = 0;
			}
		}
		return 0;
	}

	// returns the remaining quantity to add/remove. 0 if all quantity was added.
	LevelObjValue addQuantity(const ItemClass& itemClass, LevelObjValue amount, InventoryPosition invPos, Queryable* itemOwner)
	{
		for (auto& inv : (*this))
		{
			amount = inv.addQuantity(itemClass, amount, invPos, itemOwner);
			if (amount == 0)
			{
				return 0;
			}
		}
		return amount;
	}

	// Gets the sum of all the quantities for a specified item class id or 0.
	// returns true if the item is quantifiable or false (and sets quantity to 0) otherwise.
	bool getQuantity(uint16_t classIdHash16, uint32_t& quantity) const
	{
		bool isQuantifiable = false;
		uint64_t totalQuantity = 0;
		for (const auto& inv : (*this))
		{
			uint32_t quantity;
			if (inv.getQuantity(classIdHash16, quantity) == true)
			{
				totalQuantity += quantity;
				isQuantifiable = true;
			}
		}
		if (totalQuantity > std::numeric_limits<uint32_t>::max())
		{
			totalQuantity = std::numeric_limits<uint32_t>::max();
		}
		quantity = (uint32_t)totalQuantity;
		return isQuantifiable;
	}

	// returns the sum of all the quantities for a specified item class id.
	uint32_t getQuantity(uint16_t classIdHash16) const
	{
		uint32_t quantity;
		getQuantity(classIdHash16, quantity);
		return quantity;
	}

	uint32_t getMaxCapacity(const ItemClass& itemClass) const
	{
		if (itemClass.getDefaultByHash(ItemProp::Capacity) <= 0)
		{
			return 0;
		}
		uint64_t maxCapacity = 0;
		for (const auto& inv : (*this))
		{
			maxCapacity += inv.getMaxCapacity(itemClass);
		}
		if (maxCapacity >= std::numeric_limits<uint32_t>::max())
		{
			return std::numeric_limits<uint32_t>::max();
		}
		return (uint32_t)maxCapacity;
	}

	bool parseInventoryAndItem(const std::string_view str, std::string_view& props, size_t& invIdx, size_t& itemIdx) const
	{
		auto strPair = Utils::splitStringIn2(str, '.');
		invIdx = GameUtils::getPlayerInventoryIndex(strPair.first);
		if (invIdx < (*this).size())
		{
			auto strPair2 = Utils::splitStringIn2(strPair.second, '.');
			auto strPair3 = Utils::splitStringIn2(strPair2.first, ',');
			itemIdx = 0;
			if (strPair3.second.empty() == false)
			{
				size_t x = Utils::strtou(strPair3.first);
				size_t y = Utils::strtou(strPair3.second);
				const auto& invSize = (*this)[invIdx].getXYSize();
				if (x >= invSize.x || y >= invSize.y)
				{
					return false;
				}
				itemIdx = (*this)[invIdx].getIndex(x, y);
			}
			else
			{
				if (invIdx == (size_t)PlayerInventory::Body)
				{
					itemIdx = GameUtils::getPlayerItemMountIndex(strPair2.first);
				}
				else
				{
					itemIdx = Utils::strtou(strPair2.first);
				}
			}
			if (itemIdx < (*this)[invIdx].Size())
			{
				props = strPair2.second;
				return true;
			}
		}
		return false;
	}
};
