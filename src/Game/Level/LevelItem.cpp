#include "LevelItem.h"
#include "Game/Level/Level.h"
#include "Game/Player/Player.h"

Item* LevelItem::getItem(const Level& level, const PairFloat& mapCoord) noexcept
{
	if (level.map.isMapCoordValid(mapCoord) == true)
	{
		return level.map[mapCoord].getObject<Item>();
	}
	return nullptr;
}

Item* LevelItem::getItem(const Level& level, const ItemCoordInventory& itemCoord)
{
	std::shared_ptr<Player> player;
	return getItem(level, itemCoord, player);
}

Item* LevelItem::getItem(const Level& level, const ItemCoordInventory& itemCoord, std::shared_ptr<Player>& player)
{
	player = level.getPlayerOrCurrent(itemCoord.getPlayerId());
	if (player != nullptr)
	{
		if (itemCoord.isSelectedItem() == true)
		{
			return player->SelectedItem().get();
		}
		auto& inventory = player->getInventory(itemCoord.getInventoryIdx());
		if (itemCoord.isCoordXY() == true)
		{
			return inventory.get(itemCoord.getItemXY());
		}
		else
		{
			return inventory.get(itemCoord.getItemIdx());
		}
	}
	return nullptr;
}

Item* LevelItem::getItem(const Level& level, const ItemLocation& location)
{
	std::shared_ptr<Player> player;
	return getItem(level, location, player);
}

Item* LevelItem::getItem(const Level& level, const ItemLocation& location, std::shared_ptr<Player>& player)
{
	if (holdsMapCoord(location) == true)
	{
		player = nullptr;
		return getItem(level, std::get<PairFloat>(location));
	}
	else
	{
		return getItem(level, std::get<ItemCoordInventory>(location), player);
	}
}

std::shared_ptr<Item> LevelItem::removeItem(Level& level, const PairFloat& mapCoord)
{
	auto item = level.map.removeLevelObject<Item>(mapCoord);
	if (item != nullptr)
	{
		item->clearMapPosition();
		return level.levelObjects.remove<Item>(item);
	}
	return nullptr;
}

std::shared_ptr<Item> LevelItem::removeItem(Level& level, const ItemCoordInventory& itemCoord)
{
	std::shared_ptr<Player> player;
	return removeItem(level, itemCoord, player);
}

std::shared_ptr<Item> LevelItem::removeItem(Level& level, const ItemCoordInventory& itemCoord, std::shared_ptr<Player>& player)
{
	player = level.getPlayerOrCurrent(itemCoord.getPlayerId());
	if (player != nullptr)
	{
		if (itemCoord.isSelectedItem() == true)
		{
			return player->SelectedItem(nullptr);
		}
		std::shared_ptr<Item> nullItem;
		auto& inventory = player->getInventory(itemCoord.getInventoryIdx());
		if (itemCoord.isCoordXY() == true)
		{
			std::shared_ptr<Item> oldItem;
			inventory.set(itemCoord.getItemXY(), nullItem, oldItem);
			return oldItem;
		}
		else
		{
			std::shared_ptr<Item> oldItem;
			inventory.set(itemCoord.getItemIdx(), nullItem, oldItem);
			return oldItem;
		}
	}
	return nullptr;
}

std::shared_ptr<Item> LevelItem::removeItem(Level& level, const ItemLocation& location)
{
	std::shared_ptr<Player> player;
	return removeItem(level, location, player);
}

std::shared_ptr<Item> LevelItem::removeItem(Level& level, const ItemLocation& location, std::shared_ptr<Player>& player)
{
	if (holdsMapCoord(location) == true)
	{
		player = nullptr;
		return removeItem(level, std::get<PairFloat>(location));
	}
	else
	{
		return removeItem(level, std::get<ItemCoordInventory>(location), player);
	}
}

bool LevelItem::setItem(Level& level, const PairFloat& mapCoord, std::shared_ptr<Item>& item)
{
	if (level.map.isMapCoordValid(mapCoord) == false)
	{
		return false;
	}
	auto& mapCell = level.map[mapCoord];
	auto oldItem = mapCell.getObject<Item>();
	if (item == nullptr)
	{
		if (oldItem != nullptr)
		{
			oldItem->remove(level.map);
			level.levelObjects.remove<Item>(oldItem);
		}
		return true;
	}
	if (mapCell.PassableIgnoreObject(level.levelObjects.currentPlayer.lock().get()) == true
		&& oldItem == nullptr)
	{
		level.levelObjects.add(level.map, std::move(item), mapCoord);
		return true;
	}
	return false;
}

bool LevelItem::setItem(Level& level, const ItemCoordInventory& itemCoord, std::shared_ptr<Item>& item)
{
	auto player = level.getPlayerOrCurrent(itemCoord.getPlayerId());
	if (player != nullptr)
	{
		if (itemCoord.isSelectedItem() == true)
		{
			player->SelectedItem(std::move(item));
			return true;
		}
		auto invIdx = itemCoord.getInventoryIdx();
		if (invIdx < player->getInventorySize())
		{
			size_t itemIdx;
			if (itemCoord.isCoordXY() == true)
			{
				itemIdx = player->getInventory(invIdx)
					.getIndex(itemCoord.getItemXY());
			}
			else
			{
				itemIdx = itemCoord.getItemIdx();
			}
			return player->setItem(invIdx, itemIdx, item);
		}
	}
	return false;
}

bool LevelItem::setItem(Level& level, const ItemLocation& location, std::shared_ptr<Item>& item)
{
	if (holdsMapCoord(location) == true)
	{
		return setItem(level, std::get<PairFloat>(location), item);
	}
	else
	{
		return setItem(level, std::get<ItemCoordInventory>(location), item);
	}
}

bool LevelItem::deleteItem(Level& level, const PairFloat& mapCoord)
{
	std::shared_ptr<Item> nullItem;
	return setItem(level, mapCoord, nullItem);
}

bool LevelItem::deleteItem(Level& level, const ItemCoordInventory& itemCoord)
{
	std::shared_ptr<Item> nullItem;
	return setItem(level, itemCoord, nullItem);
}

bool LevelItem::deleteItem(Level& level, const ItemLocation& location)
{
	std::shared_ptr<Item> nullItem;
	return setItem(level, location, nullItem);
}

LevelObjValue LevelItem::addItemQuantity(Level& level, const ItemLocation& location, LevelObjValue amount)
{
	auto item = getItem(level, location);
	if (amount != 0 &&
		item != nullptr &&
		item->addQuantity(amount) == 0)
	{
		removeItem(level, location);
	}
	return amount;
}
