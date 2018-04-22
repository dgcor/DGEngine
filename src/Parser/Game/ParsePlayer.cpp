#include "ParsePlayer.h"
#include "Game/Player.h"
#include "GameUtils.h"
#include "ParseItem.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parsePlayerItem(Game& game, Level& level,
		const ItemCollection& inventory, Player& player, size_t invIdx, const Value& elem)
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
		if (itemIdx >= inventory.Size())
		{
			return;
		}
		auto item = parseItemObj(game, level, elem);
		player.setItem(invIdx, itemIdx, item);
	}

	void parsePlayerInventory(Game& game, Level& level,
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

		if (getBoolKey(elem, "bodyInventory") == true)
		{
			player.setBodyInventoryIdx(invIdx);
		}
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
					parsePlayerItem(game, level, inventory, player, invIdx, val);
				}
			}
			else if (itemsElem.IsObject() == true)
			{
				parsePlayerItem(game, level, inventory, player, invIdx, itemsElem);
			}
		}
	}

	void parsePlayer(Game& game, const Value& elem)
	{
		if (isValidString(elem, "class") == false)
		{
			return;
		}

		auto level = game.Resources().getLevel(getStringKey(elem, "level"));
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
		auto& mapCell = level->Map()[mapPos];

		if (mapCell.getObject<Player>() != nullptr)
		{
			return;
		}

		auto class_ = level->getPlayerClass(elem["class"].GetString());

		if (class_ == nullptr ||
			class_->hasTextures() == false)
		{
			return;
		}

		auto player = std::make_unique<Player>(class_, *level);

		player->MapPosition(mapPos);

		player->Hoverable(getBoolKey(elem, "enableHover", true));

		player->setDirection(getPlayerDirectionKey(elem, "direction"));
		player->setAnimation(getPlayerAnimationKey(elem, "animation"));
		player->setRestStatus((uint8_t)getUIntKey(elem, "restStatus"));
		player->setTextureIdx(getUIntKey(elem, "textureIndex"));

		auto outline = getColorKey(elem, "outline", class_->DefaultOutline());
		auto outlineIgnore = getColorKey(elem, "outlineIgnore", class_->DefaultOutlineIgnore());
		player->setOutline(outline, outlineIgnore);
		player->setOutlineOnHover(getBoolKey(elem, "outlineOnHover", true));

		player->setAI(getBoolKey(elem, "AI"));

		auto id = getStringKey(elem, "id");
		if (isValidId(id) == false)
		{
			id.clear();
		}
		player->Id(id);
		player->Name(getStringKey(elem, "name", class_->Name()));

		if (elem.HasMember("properties") == true)
		{
			const auto& props = elem["properties"];
			if (props.IsObject() == true)
			{
				for (auto it = props.MemberBegin(); it != props.MemberEnd(); ++it)
				{
					if (it->name.GetStringLength() > 0)
					{
						auto nameHash = str2int16(it->name.GetString());
						level->setPropertyName(nameHash, it->name.GetString());
						player->setNumberByHash(nameHash,
							getMinMaxNumber32Val(it->value), level);
					}
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

		player->updateProperties();
		player->updateTexture();

		if (getBoolKey(elem, "currentPlayer") == true)
		{
			level->setCurrentPlayer(player.get());
		}

		level->addPlayer(std::move(player));
	}
}
