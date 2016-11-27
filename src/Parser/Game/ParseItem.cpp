#include "ParseItem.h"
#include "Game/Item.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	std::shared_ptr<Item> parseItemObj(Game& game,
		const Level& level, const rapidjson::Value& elem)
	{
		if (isValidString(elem, "class") == false)
		{
			return nullptr;
		}

		auto class_ = level.getItemClass(elem["class"].GetString());
		if (class_ == nullptr)
		{
			return nullptr;
		}

		auto item = std::make_shared<Item>(class_);

		item->Hoverable(getBoolKey(elem, "enableHover", true));

		item->Name(getStringKey(elem, "name"));
		item->Description1(getStringKey(elem, "description1"));
		item->Description2(getStringKey(elem, "description2"));
		item->Description3(getStringKey(elem, "description3"));

		return item;
	}

	void parseItem(Game& game, const Value& elem)
	{
		auto level = game.Resources().getLevel(getStringKey(elem, "idLevel"));
		if (level == nullptr)
		{
			return;
		}

		bool isMapItem = (elem.HasMember("mapPosition") == true);
		bool isPlayerItem = (elem.HasMember("bodyPary") == true);

		if (isMapItem == false && isPlayerItem == false)
		{
			return;
		}

		MapCoord mapPos;
		Player* player = nullptr;

		if (isMapItem == true)
		{
			mapPos = getVector2uKey<MapCoord>(elem, "mapPosition");
			auto mapSize = level->Map().MapSize();
			if (mapPos.x >= mapSize.x || mapPos.y >= mapSize.y)
			{
				return;
			}
			if (level->Map()[mapPos.x][mapPos.y].object != nullptr)
			{
				return;
			}
		}
		else if (isPlayerItem == true)
		{
			player = level->getPlayerOrCurrent(getStringKey(elem, "idPlayer"));
			if (player != nullptr)
			{
				return;
			}
		}

		auto item = parseItemObj(game, *level, elem);
		if (item == nullptr)
		{
			return;
		}

		if (isMapItem == true)
		{
			item->MapPosition(mapPos);
			level->Map()[mapPos.x][mapPos.y].object = item;

			level->addLevelObject(item);
		}
		else if (isPlayerItem == true)
		{
			player->setInventoryItem((size_t)PlayerInventory::Body,
				(size_t)getPlayerItemMountKey(elem, "bodyPart"), item);
		}
	}
}
