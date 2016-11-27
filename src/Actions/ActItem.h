#pragma once

#include "Action.h"
#include "Game.h"
#include "Game/Level.h"
#include "Game/PlayerProperties.h"
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
				if (item != nullptr)
				{
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
	size_t itemIdx;

public:
	ActItemUpdate(const std::string& idLevel_, const std::string& idPlayer_,
		size_t invIdx_, size_t itemIdx_) : idLevel(idLevel_),
		idPlayer(idPlayer_), invIdx(invIdx_), itemIdx(itemIdx_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto player = level->getPlayerOrCurrent(idPlayer);
			if (player != nullptr)
			{
				auto selectedItem = level->SelectedItem();
				auto bodyItem = player->getInventoryItem(invIdx, itemIdx);
				if (selectedItem != nullptr)
				{
					bool hadBodyItem = (bodyItem != nullptr);
					if (player->setInventoryItem(invIdx, itemIdx, selectedItem) == true)
					{
						level->SelectedItem(bodyItem);
						game.Resources().popCursor();
						if (hadBodyItem == true)
						{
							updateCursorWithItemImage(game, *bodyItem);
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
					if (bodyItem != nullptr)
					{
						player->setInventoryItem(invIdx, itemIdx, nullptr);
						level->SelectedItem(bodyItem);
						updateCursorWithItemImage(game, *bodyItem);

						auto action = bodyItem->Class()->getActionPickInventory();
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
