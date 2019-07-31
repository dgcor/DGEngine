#include "ParsePlayer.h"
#include "Game.h"
#include "Game/Level.h"
#include "Game/Player.h"
#include "GameUtils.h"
#include "ParseItem.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parsePlayerItem(Game& game, Level& level,
		const Inventory& inventory, Player& player, size_t invIdx, const Value& elem)
	{
		if (elem.HasMember("index") == false)
		{
			return;
		}
		size_t itemIdx = 0;
		const auto& idxElem = elem["index"];
		if (idxElem.IsString() == true)
		{
			itemIdx = GameUtils::getPlayerItemMountIndex(idxElem.GetStringView());
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
			invIdx = (size_t)GameUtils::getPlayerInventory(invElem.GetStringView());
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
				auto name = Utils::toLower(getStringViewVal(classesElem));
				auto nameHash = str2int16(name);
				level.setPropertyName(nameHash, name);
				inventory.allowTypeByHash(nameHash);
			}
			else if (classesElem.IsArray() == true)
			{
				for (const auto& arrElem : classesElem)
				{
					auto name = Utils::toLower(getStringViewVal(arrElem));
					auto nameHash = str2int16(name);
					level.setPropertyName(nameHash, name);
					inventory.allowTypeByHash(nameHash);
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

	void parsePlayerSpell(Game& game, Level& level,
		Player& player, const Value& elem)
	{
		if (elem.IsObject() == false)
		{
			return;
		}
		auto id = getStringKey(elem, "class");
		auto spell = level.getClass<Spell>(id);
		if (spell == nullptr)
		{
			return;
		}
		player.addSpell(id, spell, getIntKey(elem, "level", 1));
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

		auto mapPos = getVector2UnsignedNumberKey<PairFloat, float>(elem, "mapPosition");
		if (level->Map().isMapCoordValid(mapPos) == false)
		{
			return;
		}
		auto& mapCell = level->Map()[mapPos];

		if (mapCell.getObject<Player>() != nullptr)
		{
			return;
		}

		auto class_ = level->getClass<PlayerClass>(elem["class"].GetStringStr());

		if (class_ == nullptr ||
			class_->hasTextures() == false)
		{
			return;
		}

		auto id = getStringKey(elem, "id");
		if (isValidId(id) == false)
		{
			id = {};
		}
		if (id.empty() == false && level->getLevelObject(id) != nullptr)
		{
			return;
		}

		auto player = std::make_shared<Player>(class_, *level);

		player->Id(id);

		player->Hoverable(getBoolKey(elem, "enableHover", true));

		player->setDirection(getPlayerDirectionKey(elem, "direction"));
		player->setAnimation(getPlayerAnimationKey(elem, "animation"));
		player->setTextureIdx(getUIntKey(elem, "textureIndex"));
		player->setStatus(getPlayerStatusKey(elem, "status"));
		player->setRestStatus((uint8_t)getUIntKey(elem, "restStatus"));

		auto outline = getColorKey(elem, "outline", class_->Outline());
		auto outlineIgnore = getColorKey(elem, "outlineIgnore", class_->OutlineIgnore());
		player->setOutline(outline, outlineIgnore);
		player->setOutlineOnHover(getBoolKey(elem, "outlineOnHover", true));

		player->setAI(getBoolKey(elem, "AI"));

		player->Name(getStringViewKey(elem, "name"));

		if (elem.HasMember("properties") == true)
		{
			const auto& props = elem["properties"];
			if (props.IsObject() == true)
			{
				for (auto it = props.MemberBegin(); it != props.MemberEnd(); ++it)
				{
					if (it->name.GetStringLength() > 0)
					{
						auto name = getStringViewVal(it->name);
						auto nameHash = str2int16(name);
						level->setPropertyName(nameHash, name);
						player->setNumberByHash(nameHash,
							getMinMaxNumber32Val(it->value), nullptr);
					}
				}
				player->updateLevelFromExperience(*level, false);
			}
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

		if (elem.HasMember("spell") == true)
		{
			const auto& spellElem = elem["spell"];
			if (spellElem.IsArray() == true)
			{
				for (const auto& val : spellElem)
				{
					parsePlayerSpell(game, *level, *player, val);
				}
			}
			else if (spellElem.IsObject() == true)
			{
				parsePlayerSpell(game, *level, *player, spellElem);
			}
		}

		if (elem.HasMember("selectedSpell") == true)
		{
			player->SelectedSpell(getStringVal(elem["selectedSpell"]));
		}

		player->MapPosition(*level, mapPos);
		player->updateProperties();
		player->updateTexture();

		if (getBoolKey(elem, "currentPlayer") == true)
		{
			level->setCurrentPlayer(player);
		}

		level->addLevelObject(std::move(player));
	}
}
