#include "PlayerSpells.h"

void PlayerSpells::SelectedSpell(const std::string_view id) noexcept
{
	selectedSpell = getSpellInstance(id);
}

bool PlayerSpells::hasSpell(const std::string_view key) const
{
	return spells.find(key) != spells.end();
}

void PlayerSpells::addSpell(const std::string_view key, Spell* spell, const Queryable* spellOwner, LevelObjValue spellLevel)
{
	auto it = spells.find(key);
	if (it != spells.end())
	{
		it->second.spellLevel++;
		return;
	}
	SpellInstance spellInstance(spell, spellOwner, spellLevel);
	spells.insert(std::make_pair(key, spellInstance));
}

Spell* PlayerSpells::getSpell(const std::string_view key) const
{
	auto it = spells.find(key);
	if (it != spells.end())
	{
		return it->second.spell;
	}
	return nullptr;
}

const SpellInstance* PlayerSpells::getSpellInstance(const std::string_view key) const
{
	auto it = spells.find(key);
	if (it != spells.end())
	{
		return &it->second;
	}
	return nullptr;
}
