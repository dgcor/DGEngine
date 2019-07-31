#include "ParseSpell.h"
#include "Game.h"
#include "Game/Level.h"
#include "Game/Spell.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseDescriptionValue(Spell& spell,
		const Level& level, const Value& elem)
	{
		spell.setDescription(
			getUIntKey(elem, "index"),
			level.getClassifier(getStringKey(elem, "name")),
			(uint16_t)getUIntKey(elem, "skip"));
	}

	std::unique_ptr<Spell> parseSpellHelper(const Game& game,
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

		std::unique_ptr<Spell> spell;

		if (isValidString(elem, "fromId") == true)
		{
			auto fromId = elem["fromId"].GetStringStr();
			if (fromId != id)
			{
				auto obj = level.getClass<Spell>(fromId);
				if (obj == nullptr)
				{
					return nullptr;
				}
				spell = std::make_unique<Spell>(*obj);
			}
		}

		auto texturePack1 = game.Resources().getTexturePack(
			getStringKey(elem, "texturePack1"));
		auto texturePack2 = game.Resources().getTexturePack(
			getStringKey(elem, "texturePack2"));

		auto textureIndex1 = getUIntKey(elem, "textureIndex1");
		auto textureIndex2 = getUIntKey(elem, "textureIndex2");

		if (spell == nullptr)
		{
			if (texturePack1 != nullptr &&
				texturePack2 != nullptr)
			{
				spell = std::make_unique<Spell>(
					texturePack1, texturePack2, textureIndex1, textureIndex2);
			}
		}
		else
		{
			if (texturePack1 != nullptr)
			{
				spell->setTexturePack1(texturePack1);
			}
			if (texturePack2 != nullptr)
			{
				spell->setTexturePack2(texturePack2);
			}
			if (elem.HasMember("textureIndex1") == true)
			{
				spell->setTextureIndex1(textureIndex1);
			}
			if (elem.HasMember("textureIndex2") == true)
			{
				spell->setTextureIndex2(textureIndex2);
			}
		}
		return spell;
	}

	void parseSpell(Game& game, const Value& elem)
	{
		auto level = game.Resources().getLevel(getStringKey(elem, "level"));
		if (level == nullptr)
		{
			return;
		}
		std::string id;
		auto spell = parseSpellHelper(game, *level, elem, id);
		if (spell == nullptr)
		{
			return;
		}

		spell->Id(id);

		if (elem.HasMember("name") == true)
		{
			spell->Name(getStringVal(elem["name"]));
		}
		if (elem.HasMember("type") == true)
		{
			spell->SpellType(getStringVal(elem["type"]));
		}

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
						spell->setIntByHash(nameHash,
							getMinMaxIntVal<LevelObjValue>(it->value));
					}
				}
			}
		}

		if (elem.HasMember("descriptions") == true)
		{
			const auto& descriptions = elem["descriptions"];
			if (descriptions.IsObject() == true)
			{
				parseDescriptionValue(*spell, *level, descriptions);
			}
			else if (descriptions.IsArray() == true)
			{
				for (const auto& val : descriptions)
				{
					parseDescriptionValue(*spell, *level, val);
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
					auto nameHash = str2int16(getStringViewVal(it->name));
					if (nameHash != str2int16(""))
					{
						spell->setFormula(nameHash, getStringVal(it->value));
					}
				}
			}
		}

		level->addClass(id, std::move(spell));
	}
}
