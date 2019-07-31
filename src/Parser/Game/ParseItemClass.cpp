#include "ParseItemClass.h"
#include "Game.h"
#include "Game/GameHashes.h"
#include "Game/ItemClass.h"
#include "Game/Level.h"
#include "Game/Spell.h"
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
			level.getClassifier(getStringKey(elem, "name")),
			(uint16_t)getUIntKey(elem, "skip"));
	}

	std::unique_ptr<ItemClass> parseItemClassHelper(const Game& game,
		const Level& level, const Value& elem, std::string& id)
	{
		if (isValidString(elem, "id") == false)
		{
			return nullptr;
		}
		id = elem["id"].GetStringStr();
		if (isValidId(id) == false)
		{
			return nullptr;
		}
		if (level.hasClass(id) == true)
		{
			return nullptr;
		}

		std::unique_ptr<ItemClass> itemClass;

		if (isValidString(elem, "fromId") == true)
		{
			auto fromId = elem["fromId"].GetStringStr();
			if (fromId != id)
			{
				auto obj = level.getClass<ItemClass>(fromId);
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

		auto InvTextureIdx = getUIntKey(elem, "inventoryTextureIndex");

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

		if (elem.HasMember("anchorOffset") == true)
		{
			itemClass->setAnchorOffset(
				getVector2fVal<sf::Vector2f>(elem["anchorOffset"])
			);
		}
		if (elem.HasMember("dropTextureIndexRange") == true)
		{
			itemClass->setDropTextureIndexRange(
				getVector2uVal<std::pair<uint32_t, uint32_t>>(elem["dropTextureIndexRange"])
			);
		}
		if (elem.HasMember("name") == true)
		{
			itemClass->Name(getStringViewVal(elem["name"]));
		}
		if (elem.HasMember("shortName") == true)
		{
			itemClass->ShortName(getStringViewVal(elem["shortName"]));
		}
		if (elem.HasMember("type") == true)
		{
			itemClass->Type(getStringViewVal(elem["type"]));
		}
		if (elem.HasMember("subType") == true)
		{
			itemClass->SubType(getStringViewVal(elem["subType"]));
		}
		if (elem.HasMember("inventorySize") == true)
		{
			itemClass->InventorySize(getItemXYVal(elem["inventorySize"], PairUInt8(1, 1)));
		}
		if (elem.HasMember("animationSpeed") == true)
		{
			itemClass->AnimationSpeed(getTimeVal(elem["animationSpeed"], sf::milliseconds(40)));
		}
		if (elem.HasMember("outline") == true)
		{
			itemClass->Outline(getColorVal(elem["outline"], sf::Color::Transparent));
		}
		if (elem.HasMember("outlineIgnore") == true)
		{
			itemClass->OutlineIgnore(getColorVal(elem["outlineIgnore"], sf::Color::Transparent));
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
						auto name = it->name.GetStringView();
						auto nameHash = str2int16(name);
						level->setPropertyName(nameHash, name);
						LevelObjValue val;
						switch (nameHash)
						{
						case ItemProp::UseOn:
						case ItemProp::UseOp:
						{
							auto opStr = getStringViewVal(it->value);
							val = str2int16(opStr);
							level->setPropertyName(val, opStr);
							break;
						}
						default:
							val = getMinMaxIntVal<LevelObjValue>(it->value);
							break;
						}
						itemClass->setDefault(name, val);
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
							action = itemClass->getAction(str2int16(getStringViewVal(it->value)));
						}
						if (action == nullptr)
						{
							action = parseAction(game, it->value);
						}
						itemClass->setAction(str2int16(getStringViewVal(it->name)), action);
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
					auto nameHash = str2int16(it->name.GetStringView());
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

		if (elem.HasMember("inventoryTexture") == true)
		{
			auto classifier = level->getClassifier(getStringVal(elem["inventoryTexture"]));
			itemClass->setInventoryTexture(classifier);
		}
		if (elem.HasMember("prefix") == true)
		{
			auto classifier = level->getClassifier(getStringVal(elem["prefix"]));
			itemClass->setPrefix(classifier);
		}
		if (elem.HasMember("suffix") == true)
		{
			auto classifier = level->getClassifier(getStringVal(elem["suffix"]));
			itemClass->setSuffix(classifier);
		}
		if (elem.HasMember("pricePrefix1") == true)
		{
			auto classifier = level->getClassifier(getStringVal(elem["pricePrefix1"]));
			itemClass->setPricePrefix1(classifier);
		}
		if (elem.HasMember("pricePrefix2") == true)
		{
			auto classifier = level->getClassifier(getStringVal(elem["pricePrefix2"]));
			itemClass->setPricePrefix2(classifier);
		}
		if (elem.HasMember("priceSuffix1") == true)
		{
			auto classifier = level->getClassifier(getStringVal(elem["priceSuffix1"]));
			itemClass->setPriceSuffix1(classifier);
		}
		if (elem.HasMember("priceSuffix2") == true)
		{
			auto classifier = level->getClassifier(getStringVal(elem["priceSuffix2"]));
			itemClass->setPriceSuffix2(classifier);
		}
		if (elem.HasMember("spell") == true)
		{
			auto spell = level->getClass<Spell>(getStringVal(elem["spell"]));
			itemClass->setSpell(spell);
		}

		level->addClass(id, std::move(itemClass));
	}
}
