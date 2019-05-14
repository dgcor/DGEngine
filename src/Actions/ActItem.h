#pragma once

#include "Action.h"
#include "Game.h"
#include "Game/GameProperties.h"
#include "Game/ItemLocation.h"
#include "Game/Item.h"
#include "Game/Level.h"
#include "Game/Player.h"
#include "Image.h"

static void updateCursorWithItemImage(Game& game, const Item& item)
{
	TextureInfo ti;
	if (item.getTexture(1, ti) == true)
	{
		auto image = std::make_shared<Image>();
		image->setTexture(ti, true);
		image->setOrigin();
		game.Resources().addCursor(image);
		game.updateCursorPosition();
	}
}

class ActItemAddCursor : public Action
{
private:
	std::string idLevel;
	ItemLocation itemLocation;

public:
	ActItemAddCursor(const std::string& idLevel_, const ItemLocation& itemLocation_)
		: idLevel(idLevel_), itemLocation(itemLocation_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto item = level->getItem(itemLocation);
			if (item != nullptr)
			{
				updateCursorWithItemImage(game, *item);
			}
		}
		return true;
	}
};

class ActItemAddQuantity : public Action
{
private:
	std::string idLevel;
	ItemLocation itemLocation;
	Variable quantity;
	bool remove;

public:
	ActItemAddQuantity(const std::string& idLevel_, const ItemLocation& itemLocation_,
		const Variable& quantity_, bool remove_) : idLevel(idLevel_),
		itemLocation(itemLocation_), quantity(quantity_), remove(remove_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto quantVal = (LevelObjValue)game.getVarOrPropLongV(quantity);
			if (quantVal != 0)
			{
				if (remove == true)
				{
					quantVal = -std::abs(quantVal);
				}
				level->addItemQuantity(itemLocation, quantVal);
			}
		}
		return true;
	}
};

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
			level->deleteItem(itemLocation);
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
			auto item2 = level->removeItem(itemCoord);
			auto item = item2.get();
			if (item != nullptr)
			{
				auto mapPos = level->getMapCoordOverMouse();
				if (level->setItem(mapPos, item2) == true)
				{
					item->resetDropAnimation(*level);
					game.Resources().popCursor();
					game.updateCursorPosition();

					item->Class()->executeAction(game, str2int16("levelDrop"));
				}
				else
				{
					level->setItem(itemCoord, item2);
				}
			}
		}
		return true;
	}
};

class ActItemExecuteAction : public Action
{
private:
	std::string idLevel;
	ItemLocation itemLocation;
	uint16_t actionHash16;

public:
	ActItemExecuteAction(const std::string& idLevel_,
		const ItemLocation& itemLocation_, uint16_t actionHash16_)
		: idLevel(idLevel_), itemLocation(itemLocation_), actionHash16(actionHash16_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			Player* player;
			auto item = level->getItem(itemLocation, player);

			if (holdsItemCoordInventory(itemLocation) == true &&
				std::get<ItemCoordInventory>(itemLocation).isSelectedItem() == true &&
				player != nullptr)
			{
				auto mapPos = player->MapPositionMoveTo();
				item = level->getItem(mapPos);
			}
			if (item != nullptr)
			{
				if (actionHash16 == str2int16("levelDrop"))
				{
					item->resetDropAnimation(*level);
				}
				item->Class()->executeAction(game, actionHash16);
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
	InventoryPosition invPos{ InventoryPosition::TopLeft };
	bool useInvPos{ false };
	bool splitIntoMultiple{ false };
	std::shared_ptr<Action> inventoryFullAction;

public:
	ActItemLoadFromLevel(const std::string& idLevel_,
		const ItemCoordInventory& itemCoord_, bool splitIntoMultiple_)
		: idLevel(idLevel_), itemCoord(itemCoord_), splitIntoMultiple(splitIntoMultiple_) {}

	void setInventoryPosition(InventoryPosition invPos_) noexcept
	{
		invPos = invPos_;
		useInvPos = true;
	}

	void setInventoryFullAction(const std::shared_ptr<Action>& action_) noexcept
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
				auto item2 = level->removeItem(mapPos);
				auto item = item2.get();
				if (item != nullptr)
				{
					if (itemCoord.isSelectedItem() == true &&
						player->SelectedItem() == nullptr)
					{
						player->SelectedItem(std::move(item2));
						updateCursorWithItemImage(game, *item);
						item->Class()->executeAction(game, str2int16("levelPick"));
					}
					else
					{
						if (useInvPos == false)
						{
							level->setItem(itemCoord, item2);
						}
						else
						{
							size_t invIdx = itemCoord.getInventoryIdx();
							if (invIdx < player->getInventorySize())
							{
								if (player->setItemInFreeSlot(
									invIdx, item2, invPos, splitIntoMultiple) == true)
								{
									item->Class()->executeAction(game, str2int16("levelPick"));
								}
								else
								{
									level->setItem(mapPos, item2);
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
			auto item = level->removeItem(from);
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
			Player* player = nullptr;
			auto item = level->getItem(itemLocation, player);
			if (item != nullptr)
			{
				auto prop2 = game.getVarOrPropStringS(prop);
				if (prop2.empty() == false)
				{
					auto value2 = game.getVarOrProp(value);
					item->setProperty(prop2, value2);

					if (player != nullptr)
					{
						player->updateProperties();
					}
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

	void setInventoryFullAction(const std::shared_ptr<Action>& action_) noexcept
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
					if (buyer->getFreeItemSlot(*item, invIdx, itemIdx, invPos) == true)
					{
						auto item2 = level->removeItem(itemCoord);
						level->setItem(ItemCoordInventory(idPlayer, invIdx, itemIdx), item2);
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
					auto selectedItem2 = player->SelectedItem(nullptr);
					auto selectedItem = selectedItem2.get();
					if (selectedItem != nullptr)
					{
						std::shared_ptr<Item> oldItem2;
						if (player->setItem(invIdx, itemIdx, selectedItem2, oldItem2) == true)
						{
							auto oldItem = oldItem2.get();
							player->SelectedItem(std::move(oldItem2));
							game.Resources().popCursor();
							if (oldItem != nullptr)
							{
								updateCursorWithItemImage(game, *oldItem);
							}
							else
							{
								game.updateCursorPosition();
							}
							selectedItem->Class()->executeAction(game, str2int16("inventoryDrop"));
						}
						else
						{
							player->SelectedItem(std::move(selectedItem2));
						}
					}
					else
					{
						std::shared_ptr<Item> nullItem;
						std::shared_ptr<Item> oldItem2;
						player->setItem(invIdx, itemIdx, nullItem, oldItem2);
						auto oldItem = oldItem2.get();
						if (oldItem != nullptr)
						{
							player->SelectedItem(std::move(oldItem2));
							updateCursorWithItemImage(game, *oldItem);

							oldItem->Class()->executeAction(game, str2int16("inventoryPick"));
						}
					}
				}
			}
		}
		return true;
	}
};

class ActItemUse : public Action
{
private:
	std::string idLevel;
	ItemCoordInventory itemCoord;

public:
	ActItemUse(const std::string& idLevel_, const ItemCoordInventory& itemCoord_)
		: idLevel(idLevel_), itemCoord(itemCoord_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			Player* player;
			auto item = level->getItem(itemCoord, player);
			if (item != nullptr &&
				player != nullptr)
			{
				uint32_t itemsLeft;
				if (item->use(*player, *level, itemsLeft) == true)
				{
					item->Class()->executeAction(game, str2int16("use"));
					if (itemsLeft == 0)
					{
						level->deleteItem(itemCoord);
					}
				}
			}
		}
		return true;
	}
};
