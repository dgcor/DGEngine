#pragma once

#include "Action.h"
#include "Game.h"
#include "Game/GameProperties.h"
#include "Game/Level.h"
#include "Image.h"

static void updateCursorWithItemImage(Game& game, const Item& item)
{
	auto image = std::make_shared<Image>(item.Class()->getCelInventoryTexture());
	image->setOrigin();
	game.Resources().addCursor(image);
	game.updateCursorPosition();
}

class ActItemPickFromLevel : public Action
{
private:
	std::string idLevel;
	std::string idPlayer;

public:
	ActItemPickFromLevel(const std::string& idLevel_, const std::string& idPlayer_)
		: idLevel(idLevel_), idPlayer(idPlayer_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr &&
				level->SelectedItem() == nullptr)
			{
				auto mapPos = player->MapPositionMoveTo();
				auto mapObj = level->Map()[mapPos.x][mapPos.y].object;
				if (mapObj != nullptr)
				{
					std::shared_ptr<Item> item = std::dynamic_pointer_cast<Item>(mapObj);
					if (item != nullptr)
					{
						level->Map()[mapPos.x][mapPos.y].object = nullptr;
						level->deleteLevelObject(mapObj.get());
						level->SelectedItem(item);
						updateCursorWithItemImage(game, *item);
						auto action = item->Class()->getActionPickLevel();
						if (action != nullptr)
						{
							game.Events().addBack(action);
						}
					}
				}
			}
		}
		return true;
	}
};

class ActItemDrop : public Action
{
private:
	std::string idLevel;
	std::string idPlayer;

public:
	ActItemDrop(const std::string& idLevel_, const std::string& idPlayer_)
		: idLevel(idLevel_), idPlayer(idPlayer_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto item = level->SelectedItem();
			if (item != nullptr)
			{
				auto mapPos = level->getMapCoordOverMouse();
				auto& mapCel = level->Map()[mapPos.x][mapPos.y];
				if (mapCel.Passable() == true
					&& mapCel.object == nullptr)
				{
					item->MapPosition(mapPos);
					item->updateDrawPosition(*level);
					item->resetDropAnimation();
					mapCel.object = item;
					level->addLevelObject(item);
					level->SelectedItem(nullptr);
					game.Resources().popCursor();
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
				level->SelectedItem() == nullptr)
			{
				auto mapPos = player->MapPositionMoveTo();
				auto mapObj = level->Map()[mapPos.x][mapPos.y].object;
				if (mapObj != nullptr)
				{
					std::shared_ptr<Item> item = std::dynamic_pointer_cast<Item>(mapObj);
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
		}
		return true;
	}
};

class ActItemLoadFromLevel : public Action
{
private:
	std::string idLevel;
	std::string idPlayer;
	size_t invIdx;
	InventoryPosition invPos;
	std::shared_ptr<Action> inventoryFullAction;

public:
	ActItemLoadFromLevel(const std::string& idLevel_, const std::string& idPlayer_,
		size_t invIdx_, InventoryPosition invPos_) : idLevel(idLevel_),
		idPlayer(idPlayer_), invIdx(invIdx_), invPos(invPos_) {}

	void setInventoryFullAction(const std::shared_ptr<Action>& action_)
	{
		inventoryFullAction = action_;
	}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				auto mapPos = player->MapPositionMoveTo();
				auto mapObj = level->Map()[mapPos.x][mapPos.y].object;
				if (mapObj != nullptr)
				{
					std::shared_ptr<Item> item = std::dynamic_pointer_cast<Item>(mapObj);
					if (item != nullptr)
					{
						if (invIdx < player->getInventorySize())
						{
							auto& inventory = player->getInventory(invIdx);
							auto itemSize = item->Class()->InventorySize();
							size_t itemIdx = 0;
							if (inventory.getItemSlot(itemSize, itemIdx, invPos) == true)
							{
								level->Map()[mapPos.x][mapPos.y].object = nullptr;
								level->deleteLevelObject(mapObj.get());

								inventory.set(itemIdx, item);
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

class ActItemUpdate : public Action
{
private:
	std::string idLevel;
	std::string idPlayer;
	size_t invIdx;
	sf::Vector2u itemPos;
	bool isCoordXY;

public:
	ActItemUpdate(const std::string& idLevel_, const std::string& idPlayer_,
		size_t invIdx_, size_t itemIdx_) : idLevel(idLevel_),
		idPlayer(idPlayer_), invIdx(invIdx_),
		itemPos(itemIdx_, 0), isCoordXY(false) {}

	ActItemUpdate(const std::string& idLevel_, const std::string& idPlayer_,
		size_t invIdx_, const sf::Vector2u& itemPos_) : idLevel(idLevel_),
		idPlayer(idPlayer_), invIdx(invIdx_),
		itemPos(itemPos_), isCoordXY(true) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				if (invIdx < player->getInventorySize())
				{
					auto& inventory = player->getInventory(invIdx);
					size_t itemIdx = itemPos.x;
					if (isCoordXY == true)
					{
						itemIdx = inventory.getIndex(itemPos);
					}
					auto selectedItem = level->SelectedItem();
					if (selectedItem != nullptr)
					{
						std::shared_ptr<Item> oldItem;
						if (inventory.set(itemIdx, selectedItem, oldItem) == true)
						{
							level->SelectedItem(oldItem);
							game.Resources().popCursor();
							if (oldItem != nullptr)
							{
								updateCursorWithItemImage(game, *oldItem);
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
							level->SelectedItem(oldItem);
							updateCursorWithItemImage(game, *oldItem);

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
