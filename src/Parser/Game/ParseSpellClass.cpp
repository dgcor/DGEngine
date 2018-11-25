#include "ParseSpellClass.h"
#include "Game/GameHashes.h"
#include "Game/ItemClass.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseDescriptionValue(SpellClass& spellClass,
		const Level& level, const Value& elem)
	{
		spellClass.setDescription(
			getUIntKey(elem, "index"),
			level.getClassifier(getStringKey(elem, "name")),
			(uint16_t)getUIntKey(elem, "skip"));
	}

	std::unique_ptr<SpellClass> parseSpellClassHelper(const Game& game,
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
		if (level.hasSpellClass(id) == true)
		{
			return nullptr;
		}

		std::unique_ptr<SpellClass> spellClass;

		if (isValidString(elem, "fromId") == true)
		{
			std::string fromId(elem["fromId"].GetString());
			if (fromId != id)
			{
				auto obj = level.getSpellClass(fromId);
				if (obj == nullptr)
				{
					return nullptr;
				}
				spellClass = std::make_unique<SpellClass>(*obj);
			}
		}

		auto texturePack1 = game.Resources().getTexturePack(
			getStringKey(elem, "texturePack1"));
		auto texturePack2 = game.Resources().getTexturePack(
			getStringKey(elem, "texturePack1"));

		auto textureIndex1 = (size_t)getUIntKey(elem, "textureIndex1");
		auto textureIndex2 = (size_t)getUIntKey(elem, "textureIndex2");

		if (spellClass == nullptr)
		{
			if (texturePack1 != nullptr &&
				texturePack2 != nullptr)
			{
				spellClass = std::make_unique<SpellClass>(
					texturePack1, texturePack2, textureIndex1, textureIndex2);
			}
		}
		else
		{
			if (texturePack1 != nullptr)
			{
				spellClass->setTexturePack1(texturePack1);
			}
			if (texturePack2 != nullptr)
			{
				spellClass->setTexturePack2(texturePack2);
			}
			if (elem.HasMember("textureIndex1") == true)
			{
				spellClass->setTextureIndex1(textureIndex1);
			}
			if (elem.HasMember("textureIndex2") == true)
			{
				spellClass->setTextureIndex2(textureIndex2);
			}
		}
		return spellClass;
	}

	void parseSpellClass(Game& game, const Value& elem)
	{
		auto level = game.Resources().getLevel(getStringKey(elem, "level"));
		if (level == nullptr)
		{
			return;
		}
		std::string id;
		auto spellClass = parseSpellClassHelper(game, *level, elem, id);
		if (spellClass == nullptr)
		{
			return;
		}

		spellClass->Id(id);

		if (elem.HasMember("name") == true)
		{
			spellClass->Name(getStringVal(elem["name"]));
		}
		if (elem.HasMember("type") == true)
		{
			spellClass->Type(getStringVal(elem["type"]));
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
						auto name = getStringViewVal(it->name);
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
						spellClass->setDefault(name, val);
					}
				}
			}
		}

		if (elem.HasMember("descriptions") == true)
		{
			const auto& descriptions = elem["descriptions"];
			if (descriptions.IsObject() == true)
			{
				parseDescriptionValue(*spellClass, *level, descriptions);
			}
			else if (descriptions.IsArray() == true)
			{
				for (const auto& val : descriptions)
				{
					parseDescriptionValue(*spellClass, *level, val);
				}
			}
		}

		level->addSpellClass(id, std::move(spellClass));
	}
}
