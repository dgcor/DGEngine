#pragma once

#include "Game/Item/ItemLocation.h"
#include "Game/Properties/LevelObjValue.h"
#include <memory>

class Item;
class Level;
class Player;

class LevelItem
{
public:
	static Item* getItem(const Level& level, const PairFloat& mapCoord) noexcept;
	static Item* getItem(const Level& level, const ItemCoordInventory& itemCoord);
	static Item* getItem(const Level& level, const ItemCoordInventory& itemCoord, std::shared_ptr<Player>& player);
	static Item* getItem(const Level& level, const ItemLocation& location);
	static Item* getItem(const Level& level, const ItemLocation& location, std::shared_ptr<Player>& player);

	static std::shared_ptr<Item> removeItem(Level& level, const PairFloat& mapCoord);
	static std::shared_ptr<Item> removeItem(Level& level, const ItemCoordInventory& itemCoord);
	static std::shared_ptr<Item> removeItem(Level& level, const ItemCoordInventory& itemCoord, std::shared_ptr<Player>& player);
	static std::shared_ptr<Item> removeItem(Level& level, const ItemLocation& location);
	static std::shared_ptr<Item> removeItem(Level& level, const ItemLocation& location, std::shared_ptr<Player>& player);

	static bool setItem(Level& level, const PairFloat& mapCoord, std::shared_ptr<Item>& item);
	static bool setItem(Level& level, const ItemCoordInventory& itemCoord, std::shared_ptr<Item>& item);
	static bool setItem(Level& level, const ItemLocation& location, std::shared_ptr<Item>& item);

	static bool deleteItem(Level& level, const PairFloat& mapCoord);
	static bool deleteItem(Level& level, const ItemCoordInventory& itemCoord);
	static bool deleteItem(Level& level, const ItemLocation& location);

	// returns the remaining quantity to add/remove. 0 if all quantity was added.
	static LevelObjValue addItemQuantity(Level& level, const ItemLocation& location, LevelObjValue amount);
};
