#include "PlayerInventories.h"
#include "Game/Player/PlayerBase.h"

std::shared_ptr<Item> PlayerInventories::SelectedItem(PlayerBase& player, std::shared_ptr<Item> item) noexcept
{
	auto old = std::move(selectedItem);
	selectedItem = std::move(item);
	if (selectedItem != nullptr)
	{
		selectedItem->clearMapPosition();
		selectedItem->updateOwner(&player);
	}
	if (old != nullptr)
	{
		old->updateOwner(nullptr);
	}
	return old;
}

bool PlayerInventories::hasEquipedItemType(const std::string_view type) const
{
	if (bodyInventoryIdx < size())
	{
		for (const auto& item : (*this)[bodyInventoryIdx])
		{
			if (item.ItemType() == type)
			{
				return true;
			}
		}
	}
	return false;
}

bool PlayerInventories::hasEquipedItemSubType(const std::string_view type) const
{
	if (bodyInventoryIdx >= size())
	{
		for (const auto& item : (*this)[bodyInventoryIdx])
		{
			if (item.ItemSubType() == type)
			{
				return true;
			}
		}
	}
	return false;
}

bool PlayerInventories::setItem(PlayerBase& player, PlayerInventory inv, size_t itemIdx, std::shared_ptr<Item>& item)
{
	return setItem(player, (size_t)inv, itemIdx, item);
}

bool PlayerInventories::setItem(PlayerBase& player, PlayerInventory inv,
	size_t itemIdx, std::shared_ptr<Item>& item, std::shared_ptr<Item>& oldItem)
{
	return setItem(player, (size_t)inv, itemIdx, item, oldItem);
}

bool PlayerInventories::setItem(PlayerBase& player, size_t invIdx, size_t itemIdx, std::shared_ptr<Item>& item)
{
	std::shared_ptr<Item> oldItem;
	return setItem(player, invIdx, itemIdx, item, oldItem);
}

bool PlayerInventories::setItem(PlayerBase& player, size_t invIdx,
	size_t itemIdx, std::shared_ptr<Item>& item, std::shared_ptr<Item>& oldItem)
{
	if (invIdx >= size())
	{
		return false;
	}
	auto itemPtr = item.get();
	auto& inventory = (*this)[invIdx];
	auto ret = inventory.set(itemIdx, item, oldItem);
	if (ret == true)
	{
		if (itemPtr != nullptr)
		{
			itemPtr->clearMapPosition();
			itemPtr->updateOwner(&player);
		}
		else if (oldItem != nullptr)
		{
			oldItem->updateOwner(nullptr);
		}
		if (bodyInventoryIdx == invIdx)
		{
			player.updateProperties();
		}
	}
	return ret;
}

bool PlayerInventories::setItemInFreeSlot(PlayerBase& player, PlayerInventory inv,
	std::shared_ptr<Item>& item, InventoryPosition invPos, bool splitIntoMultipleQuantities)
{
	return setItemInFreeSlot(player, (size_t)inv, item, invPos, splitIntoMultipleQuantities);
}

bool PlayerInventories::setItemInFreeSlot(PlayerBase& player, size_t invIdx,
	std::shared_ptr<Item>& item, InventoryPosition invPos, bool splitIntoMultiple)
{
	if (invIdx < size())
	{
		auto& inventory = (*this)[invIdx];

		// if item has the quantity/capacity peoperties
		if (item != nullptr &&
			item->hasPropertyByHash(ItemProp::Capacity) == true)
		{
			// first, try and fit the item into the smallest existing item of the same class
			auto quantityNeeded = item->getIntByHash(ItemProp::Quantity);
			Item* quantItem;
			if (inventory.findBiggestFreeQuantity(
				item->Class()->IdHash16(), quantItem, quantityNeeded) > 0)
			{
				LevelObjValue transferedQuantity;
				if (Inventory::updateQuantities(
					quantItem, item.get(), transferedQuantity, true) == true)
				{
					return true;
				}
			}

			// if SplitIntoMultiple is true, try and add to all free items
			// and create new items, if possible (should not create more then 1 item)
			if (splitIntoMultiple == true)
			{
				// add full quantity
				LevelObjValue itemSlots;
				auto freeSlots = inventory.getMaxCapacity(*item->Class());
				if (item->getIntByHash(ItemProp::Quantity, itemSlots) == true &&
					itemSlots >= 0 &&
					(unsigned)itemSlots <= freeSlots)
				{
					inventory.addQuantity(*item->Class(), itemSlots, invPos, &player);
					return true;
				}
				// if you can't add all of it, add none and return.
				return false;
			}
			// if it doesn't fit into the smallest, try and add it in a free slot
		}

		// try and add item to free slot
		size_t itemIdx = 0;
		if (inventory.getFreeSlot(*item, itemIdx, invPos) == true)
		{
			return setItem(player, invIdx, itemIdx, item);
		}
	}
	return false;
}
