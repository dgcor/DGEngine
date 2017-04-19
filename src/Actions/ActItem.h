#pragma once

#include "Action.h"
#include "Game.h"
#include "Game/GameProperties.h"
#include "Game/ItemLocation.h"
#include "Game/Level.h"
#include "Image.h"

static void updateCursorWithItemImage(Game& game, const Item& item, bool equiped)
{
	auto image = std::make_shared<Image>(item.Class()->getCelInventoryTexture(equiped));
	image->setOrigin();
	game.Resources().addCursor(image);
	game.updateCursorPosition();
}

class ActItemDelete : public Action
{
private:
	std::string idLevel;
	ItemLocation itemLocation;

public:
	ActItemDelete(const std::string& idLevel_, const ItemLocation& itemLocation_)
		: idLevel(idLevel_), itemLocation(itemLocation_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			level->setItem(itemLocation, nullptr);
		}
		return true;
	}
};

class ActItemDrop : public Action
{
private:
	std::string idLevel;
	ItemCoordInventory itemCoord;

public:
	ActItemDrop(const std::string& idLevel_, const ItemCoordInventory& itemCoord_)
		: idLevel(idLevel_), itemCoord(itemCoord_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto item = level->getItem(itemCoord);
			if (item != nullptr)
			{
				auto mapPos = level->getMapCoordOverMouse();
				if (level->setItem(mapPos, item) == true)
				{
					item->resetDropAnimation();
					level->setItem(itemCoord, nullptr);
					if (game.Resources().cursorCount() > 1)
					{
						game.Resources().popCursor();
					}
					game.updateCursorPosition();

					auto action = item->Class()->getActionDropLevel();
					if (action != nullptr)
					{
						game.Events().addBack(action);
					}
				}
			}
		}
		return true;
	}
};

class ActItemExecuteDropAction : public Action
{
private:
	std::string idLevel;
	std::string idPlayer;

public:
	ActItemExecuteDropAction(const std::string& idLevel_, const std::string& idPlayer_)
		: idLevel(idLevel_), idPlayer(idPlayer_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr &&
				player->SelectedItem() == nullptr)
			{
				auto mapPos = player->MapPositionMoveTo();
				auto item = level->getItem(mapPos);
				if (item != nullptr)
				{
					item->resetDropAnimation();
					auto action = item->Class()->getActionDropLevel();
					if (action != nullptr)
					{
						game.Events().addBack(action);
					}
				}
			}
		}
		return true;
	}
};

class ActItemLoadFromLevel : public Action
{
private:
	std::string idLevel;
	ItemCoordInventory itemCoord;
	InventoryPosition invPos;
	bool useInvPos{ false };
	std::shared_ptr<Action> inventoryFullAction;

	void executePickFromLevelAction(Game& game, Item& item)
	{
		auto action = item.Class()->getActionPickLevel();
		if (action != nullptr)
		{
			game.Events().addBack(action);
		}
	}

public:
	ActItemLoadFromLevel(const std::string& idLevel_,
		const ItemCoordInventory& itemCoord_)
		: idLevel(idLevel_), itemCoord(itemCoord_) {}

	void setInventoryPosition(InventoryPosition invPos_)
	{
		invPos = invPos_;
		useInvPos = true;
	}

	void setInventoryFullAction(const std::shared_ptr<Action>& action_)
	{
		inventoryFullAction = action_;
	}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(itemCoord.getPlayerId());
			if (player != nullptr)
			{
				auto mapPos = player->MapPositionMoveTo();
				auto item = level->getItem(mapPos);
				if (item != nullptr)
				{
					if (itemCoord.isSelectedItem() == true &&
						player->SelectedItem() == nullptr)
					{
						level->setItem(mapPos, nullptr);
						player->SelectedItem(item);
						updateCursorWithItemImage(game, *item,
							player->canEquipItem(*item));
						executePickFromLevelAction(game, *item);
					}
					else
					{
						if (useInvPos == false)
						{
							level->setItem(mapPos, nullptr);
							level->setItem(itemCoord, item);
						}
						else if (itemCoord.getInventoryIdx() < player->getInventorySize())
						{
							auto& inventory = player->getInventory(itemCoord.getInventoryIdx());
							size_t itemIdx = 0;
							if (inventory.getItemSlot(*item, itemIdx, invPos) == true)
							{
								level->setItem(mapPos, nullptr);
								inventory.set(itemIdx, item);
								player->updateGoldAdd(item);
								executePickFromLevelAction(game, *item);
							}
							else
							{
								if (inventoryFullAction != nullptr)
								{
									game.Events().addBack(inventoryFullAction);
								}
							}
						}
					}
				}
			}
		}
		return true;
	}
};

class ActItemMove : public Action
{
private:
	std::string idLevel;
	ItemLocation from;
	ItemLocation to;

public:
	ActItemMove(const std::string& idLevel_, const ItemLocation& from_,
		const ItemLocation& to_) : idLevel(idLevel_), from(from_), to(to_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto item = level->getItem(from);
			if (item != nullptr)
			{
				level->setItem(from, nullptr);
			}
			level->setItem(to, item);
		}
		return true;
	}
};

class ActItemSetProperty : public Action
{
private:
	ItemLocation itemLocation;
	std::string idLevel;
	std::string prop;
	Variable value;

public:
	ActItemSetProperty(const ItemLocation& itemLocation_, const std::string& idLevel_,
		const std::string& prop_, const Variable& value_) : itemLocation(itemLocation_),
		idLevel(idLevel_), prop(prop_), value(value_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto item = level->getItem(itemLocation);
			if (item != nullptr)
			{
				Variable prop2(prop);
				game.getVarOrProp(prop, prop2);
				if (prop2.is<std::string>() == true)
				{
					const auto& propVal = prop2.get<std::string>();
					auto value2 = value;
					if (value2.is<std::string>() == true)
					{
						game.getVarOrProp(value2.get<std::string>(), value2);
					}
					item->setProperty(propVal, value2);
				}
			}
		}
		return true;
	}
};

class ActItemTrade : public Action
{
private:
	std::string idLevel;
	std::string idPlayer;
	ItemCoordInventory itemCoord;
	InventoryPosition invPos;
	std::shared_ptr<Action> inventoryFullAction;

public:
	ActItemTrade(const std::string& idLevel_, const std::string& idPlayer_,
		const ItemCoordInventory& itemCoord_, InventoryPosition invPos_)
		: idLevel(idLevel_), idPlayer(idPlayer_), itemCoord(itemCoord_),
		invPos(invPos_) {}

	void setInventoryFullAction(const std::shared_ptr<Action>& action_)
	{
		inventoryFullAction = action_;
	}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto buyer = level->getPlayerOrCurrent(idPlayer);
			auto seller = level->getPlayerOrCurrent(itemCoord.getPlayerId());
			if (buyer != nullptr &&
				seller != nullptr &&
				buyer != seller)
			{
				auto item = level->getItem(itemCoord);
				if (item != nullptr)
				{
					size_t invIdx = 0;
					size_t itemIdx = 0;
					if (buyer->getItemSlot(*item, invIdx, itemIdx, invPos) == true)
					{
						level->setItem(ItemCoordInventory(idPlayer, invIdx, itemIdx), item);
						level->setItem(itemCoord, nullptr);
					}
					else
					{
						if (inventoryFullAction != nullptr)
						{
							game.Events().addBack(inventoryFullAction);
						}
					}
				}
			}
		}
		return true;
	}
};

class ActItemUpdate : public Action
{
private:
	std::string idLevel;
	ItemCoordInventory itemCoord;

public:
	ActItemUpdate(const std::string& idLevel_, const ItemCoordInventory& itemCoord_)
		: idLevel(idLevel_), itemCoord(itemCoord_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr &&
			itemCoord.isSelectedItem() == false)
		{
			auto player = level->getPlayerOrCurrent(itemCoord.getPlayerId());
			if (player != nullptr)
			{
				auto invIdx = itemCoord.getInventoryIdx();
				if (invIdx < player->getInventorySize())
				{
					auto& inventory = player->getInventory(invIdx);
					size_t itemIdx;
					if (itemCoord.isCoordXY() == true)
					{
						itemIdx = inventory.getIndex(itemCoord.getItemXY());
					}
					else
					{
						itemIdx = itemCoord.getItemIdx();
					}
					auto selectedItem = player->SelectedItem();
					if (selectedItem != nullptr)
					{
						std::shared_ptr<Item> oldItem;
						if (inventory.set(itemIdx, selectedItem, oldItem) == true)
						{
							player->updateGoldAdd(selectedItem);
							player->updateGoldRemove(oldItem);
							player->SelectedItem(oldItem);
							if (game.Resources().cursorCount() > 1)
							{
								game.Resources().popCursor();
							}
							if (oldItem != nullptr)
							{
								updateCursorWithItemImage(game, *oldItem,
									player->canEquipItem(*oldItem));
							}
							else
							{
								game.updateCursorPosition();
							}
							auto action = selectedItem->Class()->getActionDropInventory();
							if (action != nullptr)
							{
								game.Events().addBack(action);
							}
						}
					}
					else
					{
						auto oldItem = inventory.get(itemIdx);
						if (oldItem != nullptr)
						{
							inventory.set(itemIdx, nullptr);
							player->updateGoldRemove(oldItem);
							player->SelectedItem(oldItem);
							updateCursorWithItemImage(game, *oldItem,
								player->canEquipItem(*oldItem));

							auto action = oldItem->Class()->getActionPickInventory();
							if (action != nullptr)
							{
								game.Events().addBack(action);
							}
						}
					}
				}
			}
		}
		return true;
	}
};
