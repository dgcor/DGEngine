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
	using namespace std::literals;

	void parseDescriptionValue(ItemClass& itemClass,
		const Level& level, const Value& elem)
	{
		itemClass.setDescription(
			getUIntKey(elem, "index"),
			level.getClassifier(getStringViewKey(elem, "name")),
			(uint16_t)getUIntKey(elem, "skip"));
	}

	std::unique_ptr<ItemClass> parseItemClassHelper(const Game& game,
		const Level& level, const Value& elem, std::string& id)
	{
		if (isValidString(elem, "id") == false)
		{
			return nullptr;
		}
		id = elem["id"sv].GetStringView();
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
			auto fromId = elem["fromId"sv].GetStringView();
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
			getStringViewKey(elem, "dropTexturePack"));
		auto InvTexture = game.Resources().getTexturePack(
			getStringViewKey(elem, "inventoryTexturePack"));

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
			if (elem.HasMember("inventoryTextureIndex"sv) == true)
			{
				itemClass->setInventoryTextureIndex(InvTextureIdx);
			}
		}
		return itemClass;
	}

	void parseItemClass(Game& game, const Value& elem)
	{
		auto level = game.Resources().getLevel(getStringViewKey(elem, "level"));
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

		if (elem.HasMember("anchorOffset"sv) == true)
		{
			itemClass->setAnchorOffset(
				getVector2fVal<sf::Vector2f>(elem["anchorOffset"sv])
			);
		}
		if (elem.HasMember("dropTextureIndexRange"sv) == true)
		{
			itemClass->setDropTextureIndexRange(
				getVector2uVal<std::pair<uint32_t, uint32_t>>(elem["dropTextureIndexRange"sv])
			);
		}
		if (elem.HasMember("name"sv) == true)
		{
			itemClass->Name(getStringViewVal(elem["name"sv]));
		}
		if (elem.HasMember("shortName"sv) == true)
		{
			itemClass->ShortName(getStringViewVal(elem["shortName"sv]));
		}
		if (elem.HasMember("type"sv) == true)
		{
			itemClass->Type(getStringViewVal(elem["type"sv]));
		}
		if (elem.HasMember("subType"sv) == true)
		{
			itemClass->SubType(getStringViewVal(elem["subType"sv]));
		}
		if (elem.HasMember("inventorySize"sv) == true)
		{
			itemClass->InventorySize(getItemXYVal(elem["inventorySize"sv], PairUInt8(1, 1)));
		}
		if (elem.HasMember("animationSpeed"sv) == true)
		{
			itemClass->AnimationSpeed(getTimeVal(elem["animationSpeed"sv], sf::milliseconds(40)));
		}
		if (elem.HasMember("outline"sv) == true)
		{
			itemClass->Outline(getColorVal(elem["outline"sv], sf::Color::Transparent));
		}
		if (elem.HasMember("outlineIgnore"sv) == true)
		{
			itemClass->OutlineIgnore(getColorVal(elem["outlineIgnore"sv], sf::Color::Transparent));
		}

		if (elem.HasMember("defaults"sv) == true)
		{
			const auto& defaults = elem["defaults"sv];
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

		if (elem.HasMember("actions"sv) == true)
		{
			const auto& actions = elem["actions"sv];
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
							action = getActionVal(game, it->value);
						}
						itemClass->setAction(str2int16(getStringViewVal(it->name)), action);
					}
				}
			}
		}

		if (elem.HasMember("formulas"sv) == true)
		{
			const auto& formulas = elem["formulas"sv];
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
							itemClass->setFormula(nameHash, getStringViewVal(it->value));
						}
					}
				}
			}
		}

		if (elem.HasMember("descriptions"sv) == true)
		{
			const auto& descriptions = elem["descriptions"sv];
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

		if (elem.HasMember("inventoryTexture"sv) == true)
		{
			auto classifier = level->getClassifier(getStringViewVal(elem["inventoryTexture"sv]));
			itemClass->setInventoryTexture(classifier);
		}
		if (elem.HasMember("prefix"sv) == true)
		{
			auto classifier = level->getClassifier(getStringViewVal(elem["prefix"sv]));
			itemClass->setPrefix(classifier);
		}
		if (elem.HasMember("suffix"sv) == true)
		{
			auto classifier = level->getClassifier(getStringViewVal(elem["suffix"sv]));
			itemClass->setSuffix(classifier);
		}
		if (elem.HasMember("pricePrefix1"sv) == true)
		{
			auto classifier = level->getClassifier(getStringViewVal(elem["pricePrefix1"sv]));
			itemClass->setPricePrefix1(classifier);
		}
		if (elem.HasMember("pricePrefix2"sv) == true)
		{
			auto classifier = level->getClassifier(getStringViewVal(elem["pricePrefix2"sv]));
			itemClass->setPricePrefix2(classifier);
		}
		if (elem.HasMember("priceSuffix1"sv) == true)
		{
			auto classifier = level->getClassifier(getStringViewVal(elem["priceSuffix1"sv]));
			itemClass->setPriceSuffix1(classifier);
		}
		if (elem.HasMember("priceSuffix2"sv) == true)
		{
			auto classifier = level->getClassifier(getStringViewVal(elem["priceSuffix2"sv]));
			itemClass->setPriceSuffix2(classifier);
		}
		if (elem.HasMember("spell"sv) == true)
		{
			auto spell = level->getClass<Spell>(getStringViewVal(elem["spell"sv]));
			itemClass->setSpell(spell);
		}

		level->addClass(id, std::move(itemClass));
	}
}
