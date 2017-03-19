#include "ParsePlayer.h"
#include "Game/Player.h"
#include "GameUtils.h"
#include "ParseItem.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parsePlayerItem(Game& game, const Level& level,
		ItemCollection& inventory, const Value& elem)
	{
		if (elem.HasMember("index") == false)
		{
			return;
		}
		size_t itemIdx = 0;
		const auto& idxElem = elem["index"];
		if (idxElem.IsString() == true)
		{
			itemIdx = (size_t)GameUtils::getPlayerItemMountIndex(idxElem.GetString());
		}
		else if (idxElem.IsArray() == true)
		{
			itemIdx = inventory.getIndex(getItemXYVal(idxElem));
		}
		else if (idxElem.IsUint() == true)
		{
			itemIdx = idxElem.GetUint();
		}
		else
		{
			return;
		}
		auto item = parseItemObj(game, level, elem);
		if (itemIdx < inventory.Size())
		{
			inventory.set(itemIdx, item);
		}
	}

	void parsePlayerInventory(Game& game, const Level& level,
		Player& player, const Value& elem)
	{
		if (elem.HasMember("index") == false)
		{
			return;
		}

		size_t invIdx = 0;
		const auto& invElem = elem["index"];
		if (invElem.IsString() == true)
		{
			invIdx = (size_t)GameUtils::getPlayerInventory(invElem.GetString());
		}
		else if (invElem.IsUint() == true)
		{
			invIdx = invElem.GetUint();
		}
		if (invIdx >= player.getInventorySize())
		{
			return;
		}
		auto& inventory = player.getInventory(invIdx);
		bool wasInitialized = false;
		if (elem.HasMember("size") == true)
		{
			const auto& sizeElem = elem["size"];
			if (sizeElem.IsUint() == true)
			{
				inventory.init(sizeElem.GetUint());
				wasInitialized = true;
			}
			else if (sizeElem.IsArray() == true)
			{
				inventory.init(getItemXYVal(sizeElem));
				wasInitialized = true;
			}
		}
		if (wasInitialized == false
			&& invIdx == (size_t)PlayerInventory::Body)
		{
			inventory.init((size_t)PlayerItemMount::Size);
		}
		if (inventory.Size() == 0)
		{
			return;
		}

		inventory.setEnforceItemSize(getBoolKey(elem, "enforceItemSize"));

		if (elem.HasMember("allowedClassTypes") == true)
		{
			const auto& classesElem = elem["allowedClassTypes"];
			if (classesElem.IsString() == true)
			{
				inventory.allowType(Utils::toLower(classesElem.GetString()));
			}
			else if (classesElem.IsArray() == true)
			{
				for (const auto& arrElem : classesElem)
				{
					inventory.allowType(Utils::toLower(getStringVal(arrElem)));
				}
			}
		}
		if (elem.HasMember("item") == true)
		{
			const auto& itemsElem = elem["item"];
			if (itemsElem.IsArray() == true)
			{
				for (const auto& val : itemsElem)
				{
					parsePlayerItem(game, level, inventory, val);
				}
			}
			else if (itemsElem.IsObject() == true)
			{
				parsePlayerItem(game, level, inventory, itemsElem);
			}
		}
	}

	void parsePlayer(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false
			|| isValidString(elem, "class") == false)
		{
			return;
		}
		std::string id(elem["id"].GetString());
		if (isValidId(id) == false)
		{
			return;
		}

		auto level = game.Resources().getLevel(getStringKey(elem, "idLevel"));
		if (level == nullptr)
		{
			return;
		}

		auto mapPos = getVector2uKey<MapCoord>(elem, "mapPosition");
		auto mapSize = level->Map().MapSize();
		if (mapPos.x >= mapSize.x || mapPos.y >= mapSize.y)
		{
			return;
		}
		auto& mapCell = level->Map()[mapPos.x][mapPos.y];

		if (mapCell.object != nullptr)
		{
			return;
		}

		auto class_ = level->getPlayerClass(elem["class"].GetString());
		if (class_ == nullptr)
		{
			return;
		}

		auto player = std::make_shared<Player>(class_);

		player->applyDefaults();

		player->MapPosition(mapPos);
		mapCell.object = player;

		player->Hoverable(getBoolKey(elem, "enableHover", true));

		player->setDirection(getPlayerDirectionKey(elem, "direction"));
		player->setStatus(getPlayerStatusKey(elem, "status"));
		player->setPalette(getUIntKey(elem, "palette"));

		player->Id(id);
		player->Name(getStringKey(elem, "name"));
		player->CurrentLevel(getIntKey(elem, "level"));
		player->Experience(getIntKey(elem, "experience"));
		player->ExpNextLevel(getIntKey(elem, "expNextLevel"));
		player->Points(getIntKey(elem, "points"));
		player->Gold(getIntKey(elem, "gold"));

		if (elem.HasMember("properties") == true)
		{
			const auto& props = elem["properties"];
			if (props.IsObject() == true)
			{
				for (auto it = props.MemberBegin(); it != props.MemberEnd(); ++it)
				{
					player->setPlayerProperty(it->name.GetString(),
						getMinMaxIntVal<int16_t>(it->value));
				}
			}
		}

		if (elem.HasMember("action") == true)
		{
			player->setAction(parseAction(game, elem["action"]));
		}

		if (elem.HasMember("inventory") == true)
		{
			const auto& invElem = elem["inventory"];
			if (invElem.IsArray() == true)
			{
				for (const auto& val : invElem)
				{
					parsePlayerInventory(game, *level, *player, val);
				}
			}
			else if (invElem.IsObject() == true)
			{
				parsePlayerInventory(game, *level, *player, invElem);
			}
		}

		player->updatePlayerProperties(PlayerInventory::Body);

		level->addPlayer(player);

		if (getBoolKey(elem, "currentPlayer") == true)
		{
			level->setCurrentPlayer(player.get());
		}
	}
}
