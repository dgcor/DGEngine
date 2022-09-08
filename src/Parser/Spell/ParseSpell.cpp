#include "ParseSpell.h"
#include "Game/Game.h"
#include "Game/Level/Level.h"
#include "Game/Spell/Spell.h"
#include "Parser/LevelObject/ParseLevelObjectCommon.h"
#include "Parser/LevelObject/ParseLevelObjectClassCommon.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::unique_ptr<Spell> parseSpellHelper(const Game& game, const Level& level, const Value& elem, std::string& id)
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
		if (level.LevelObjects().hasClass(id) == true)
		{
			return nullptr;
		}

		std::unique_ptr<Spell> spell;

		if (isValidString(elem, "fromId") == true)
		{
			auto fromId = elem["fromId"sv].GetStringView();
			if (fromId != id)
			{
				auto obj = level.LevelObjects().getClass<Spell>(fromId);
				if (obj == nullptr)
				{
					return nullptr;
				}
				spell = std::make_unique<Spell>(*obj);
			}
		}

		auto texturePack1 = game.Resources().getTexturePack(getStringViewKey(elem, "texturePack1"));
		auto texturePack2 = game.Resources().getTexturePack(getStringViewKey(elem, "texturePack2"));

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
			if (elem.HasMember("textureIndex1"sv) == true)
			{
				spell->setTextureIndex1(textureIndex1);
			}
			if (elem.HasMember("textureIndex2"sv) == true)
			{
				spell->setTextureIndex2(textureIndex2);
			}
		}
		return spell;
	}

	void parseSpell(Game& game, const Value& elem)
	{
		auto level = game.Resources().getLevel<Level>(getStringViewKey(elem, "level"));
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

		if (elem.HasMember("name"sv) == true)
		{
			spell->Name(getStringViewVal(elem["name"sv]));
		}
		if (elem.HasMember("type"sv) == true)
		{
			spell->SpellType(getStringViewVal(elem["type"sv]));
		}

		parseLevelObjectProperties(*spell, *level, elem);
		parseLevelObjectClassDescriptions(*spell, *level, elem);
		parseLevelObjectClassFormulas(*spell, elem);

		level->LevelObjects().addClass(id, std::move(spell));
	}
}
