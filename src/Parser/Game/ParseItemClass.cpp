#include "ParseItemClass.h"
#include "Game/ItemProperties.h"
#include "Game/ItemClass.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseDescriptionValue(ItemClass& itemClass,
		const Level& level, const Value& elem)
	{
		itemClass.setDescription(
			getUIntKey(elem, "index"),
			level.getNamer(getStringKey(elem, "name")),
			(uint16_t)getUIntKey(elem, "skip"));
	}

	std::unique_ptr<ItemClass> parseItemClassHelper(const Game& game,
		const Level& level, const Value& elem, std::string& id)
	{
		if (isValidString(elem, "id") == false)
		{
			return nullptr;
		}
		id = std::string(elem["id"].GetString());
		if (isValidId(id) == false)
		{
			return nullptr;
		}
		if (level.hasItemClass(id) == true)
		{
			return nullptr;
		}

		std::unique_ptr<ItemClass> itemClass;

		if (isValidString(elem, "fromId") == true)
		{
			std::string fromId(elem["fromId"].GetString());
			if (fromId != id)
			{
				auto obj = level.getItemClass(fromId);
				if (obj == nullptr)
				{
					return nullptr;
				}
				itemClass = std::make_unique<ItemClass>(*obj);
			}
		}

		auto dropTexture = game.Resources().getTexturePack(
			getStringKey(elem, "dropTexturePack"));
		auto InvTexture = game.Resources().getTexturePack(
			getStringKey(elem, "inventoryTexturePack"));

		auto InvTextureIdx = (size_t)getUIntKey(elem, "inventoryTextureIndex");

		if (itemClass == nullptr)
		{
			if (dropTexture != nullptr &&
				InvTexture != nullptr)
			{
				itemClass = std::make_unique<ItemClass>(dropTexture, InvTexture, InvTextureIdx);
			}
		}
		else
		{
			if (dropTexture != nullptr)
			{
				itemClass->setDropTexturePack(dropTexture);
			}
			if (InvTexture != nullptr)
			{
				itemClass->setInventoryTexturePack(InvTexture);
			}
			if (elem.HasMember("inventoryTextureIndex") == true)
			{
				itemClass->setInventoryTextureIndex(InvTextureIdx);
			}
		}
		return itemClass;
	}

	void parseItemClass(Game& game, const Value& elem)
	{
		auto level = game.Resources().getLevel(getStringKey(elem, "level"));
		if (level == nullptr)
		{
			return;
		}
		std::string id;
		auto itemClass = parseItemClassHelper(game, *level, elem, id);
		if (itemClass == nullptr)
		{
			return;
		}

		itemClass->Id(id);

		if (elem.HasMember("dropTextureIndexRange") == true)
		{
			itemClass->setDropTextureIndexRange(
				getVector2uVal<std::pair<size_t, size_t>>(elem["dropTextureIndexRange"])
			);
		}

		if (elem.HasMember("name") == true)
		{
			itemClass->Name(getStringVal(elem["name"]));
		}
		if (elem.HasMember("shortName") == true)
		{
			itemClass->ShortName(getStringVal(elem["shortName"]));
		}
		if (elem.HasMember("type") == true)
		{
			itemClass->Type(getStringVal(elem["type"]));
		}
		if (elem.HasMember("subType") == true)
		{
			itemClass->SubType(getStringVal(elem["subType"]));
		}
		if (elem.HasMember("inventorySize") == true)
		{
			itemClass->InventorySize(getItemXYVal(elem["inventorySize"], ItemXY(1, 1)));
		}
		if (elem.HasMember("defaultAnimationSpeed") == true)
		{
			itemClass->DefaultAnimationSpeed(getTimeVal(elem["defaultAnimationSpeed"], sf::milliseconds(40)));
		}
		if (elem.HasMember("defaultOutline") == true)
		{
			itemClass->DefaultOutline(getColorVal(elem["defaultOutline"], sf::Color::Transparent));
		}
		if (elem.HasMember("defaultOutlineIgnore") == true)
		{
			itemClass->DefaultOutlineIgnore(getColorVal(elem["defaultOutlineIgnore"], sf::Color::Transparent));
		}

		if (elem.HasMember("defaults") == true)
		{
			const auto& defaults = elem["defaults"];
			if (defaults.IsObject() == true)
			{
				for (auto it = defaults.MemberBegin(); it != defaults.MemberEnd(); ++it)
				{
					if (it->name.GetStringLength() > 0)
					{
						auto nameStr = it->name.GetString();
						auto nameHash = str2int16(nameStr);
						level->setPropertyName(nameHash, nameStr);
						LevelObjValue val;
						switch (nameHash)
						{
						case ItemProp::UseOn:
						case ItemProp::UseOp:
						{
							auto opStr = getStringCharVal(it->value);
							val = str2int16(opStr);
							level->setPropertyName(val, opStr);
							break;
						}
						default:
							val = getMinMaxIntVal<LevelObjValue>(it->value);
							break;
						}
						itemClass->setDefault(nameStr, val);
					}
				}
			}
		}

		if (elem.HasMember("actions") == true)
		{
			const auto& actions = elem["actions"];
			if (actions.IsObject() == true)
			{
				for (auto it = actions.MemberBegin(); it != actions.MemberEnd(); ++it)
				{
					if (it->name.GetStringLength() > 0)
					{
						std::shared_ptr<Action> action;
						if (it->value.IsString() == true)
						{
							action = itemClass->getAction(str2int16(it->value.GetString()));
						}
						if (action == nullptr)
						{
							action = parseAction(game, it->value);
						}
						itemClass->setAction(str2int16(it->name.GetString()), action);
					}
				}
			}
		}

		if (elem.HasMember("formulas") == true)
		{
			const auto& formulas = elem["formulas"];
			if (formulas.IsObject() == true)
			{
				for (auto it = formulas.MemberBegin(); it != formulas.MemberEnd(); ++it)
				{
					auto nameHash = str2int16(it->name.GetString());
					if (nameHash != str2int16(""))
					{
						if (it->value.IsNull() == true)
						{
							itemClass->deleteFormula(nameHash);
						}
						else
						{
							itemClass->setFormula(nameHash, getStringVal(it->value));
						}
					}
				}
			}
		}

		if (elem.HasMember("descriptions") == true)
		{
			const auto& descriptions = elem["descriptions"];
			if (descriptions.IsObject() == true)
			{
				parseDescriptionValue(*itemClass, *level, descriptions);
			}
			else if (descriptions.IsArray() == true)
			{
				for (const auto& val : descriptions)
				{
					parseDescriptionValue(*itemClass, *level, val);
				}
			}
		}

		if (elem.HasMember("prefix") == true)
		{
			auto namer = level->getNamer(getStringVal(elem["prefix"]));
			itemClass->setPrefix(namer);
		}
		if (elem.HasMember("suffix") == true)
		{
			auto namer = level->getNamer(getStringVal(elem["suffix"]));
			itemClass->setSuffix(namer);
		}

		level->addItemClass(id, std::move(itemClass));
	}
}
