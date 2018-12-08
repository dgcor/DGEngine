#pragma once

#include <array>
#include "GameHashes.h"
#include "Inventory.h"

template <size_t Size_>
class Inventories : private std::array<Inventory, Size_>
{
	typedef std::array<Inventory, Size_> InvArray;

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

	bool getFreeSlot(const Item& item, size_t& invIdx,
		size_t& itemIdx, InventoryPosition invPos) const
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

	bool findByClass(uint16_t classIdHash16, size_t& invIdx,
		size_t& itemIdx, Item*& item) const
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

	uint32_t findFreeQuantity(uint16_t classIdHash16,
		size_t& invIdx, size_t& itemIdx, Item*& item) const
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
	LevelObjValue addQuantity(const ItemClass& itemClass,
		LevelObjValue amount, InventoryPosition invPos)
	{
		for (auto& inv : (*this))
		{
			amount = inv.addQuantity(itemClass, amount, invPos);
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
};
