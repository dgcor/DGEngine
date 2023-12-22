#pragma once

#include "Game/Inventory/Inventories.h"

class PlayerBase;

class PlayerInventories : private Inventories<(size_t)PlayerInventory::Size>
{
private:
	using InvArray = Inventories<(size_t)PlayerInventory::Size>;

	size_t bodyInventoryIdx{ (size_t)PlayerInventory::Size };
	std::shared_ptr<Item> selectedItem;

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

	using InvArray::empty;
	using InvArray::getFreeSlot;
	using InvArray::hasFreeSlot;
	using InvArray::hasItem;
	using InvArray::addQuantity;
	using InvArray::getQuantity;
	using InvArray::getMaxCapacity;
	using InvArray::parseInventoryAndItem;

	constexpr auto hasBodyInventory() const noexcept { return bodyInventoryIdx < size(); }
	auto getBodyInventoryIdx() const noexcept { return bodyInventoryIdx; }
	void setBodyInventoryIdx(size_t idx) noexcept { bodyInventoryIdx = std::min(idx, (size_t)PlayerInventory::Size); }

	auto& SelectedItem() const noexcept { return selectedItem; }
	std::shared_ptr<Item> SelectedItem(PlayerBase& player, std::shared_ptr<Item> item) noexcept;

	bool hasEquipedItemType(const std::string_view type) const;
	bool hasEquipedItemSubType(const std::string_view type) const;

	bool setItem(PlayerBase& player, PlayerInventory inv, size_t itemIdx, std::shared_ptr<Item>& item);

	bool setItem(PlayerBase& player, PlayerInventory inv, size_t itemIdx, std::shared_ptr<Item>& item, std::shared_ptr<Item>& oldItem);

	bool setItem(PlayerBase& player, size_t invIdx, size_t itemIdx, std::shared_ptr<Item>& item);

	bool setItem(PlayerBase& player, size_t invIdx, size_t itemIdx, std::shared_ptr<Item>& item, std::shared_ptr<Item>& oldItem);

	bool setItemInFreeSlot(PlayerBase& player, PlayerInventory inv, std::shared_ptr<Item>& item,
		InventoryPosition invPos, bool splitIntoMultipleQuantities);

	// sets an item in a free slot. If the item has quantity/capacity
	// it tries to fit into the smallest quantity item of the same class first
	// if splitIntoMultiple is true, it tries to fit into all items of the same class.
	bool setItemInFreeSlot(PlayerBase& player, size_t invIdx, std::shared_ptr<Item>& item,
		InventoryPosition invPos, bool splitIntoMultiple);
};
