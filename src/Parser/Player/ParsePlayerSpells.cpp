#include "ParsePlayerSpells.h"
#include "Game/Level/Level.h"
#include "Game/Player/Player.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parsePlayerSpell(Player& player, Level& level, const Value& elem)
	{
		if (elem.IsObject() == false)
		{
			return;
		}
		auto id = getStringViewKey(elem, "class");
		auto spell = level.LevelObjects().getClass<Spell>(id);
		if (spell == nullptr)
		{
			return;
		}
		player.addSpell(id, spell, getIntKey(elem, "level", 1));
	}

	void parsePlayerSpells(Player& player, Level& level, const Value& elem)
	{
		if (elem.HasMember("spell"sv) == true)
		{
			const auto& spellElem = elem["spell"sv];
			if (spellElem.IsArray() == true)
			{
				for (const auto& val : spellElem)
				{
					parsePlayerSpell(player, level, val);
				}
			}
			else if (spellElem.IsObject() == true)
			{
				parsePlayerSpell(player, level, spellElem);
			}
		}

		if (elem.HasMember("selectedSpell"sv) == true)
		{
			player.SelectedSpell(getStringViewVal(elem["selectedSpell"sv]));
		}
	}
}
