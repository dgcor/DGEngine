#pragma once

#include "Game/Spell/SpellInstance.h"
#include "Utils/UnorderedStringMap.h"

class PlayerSpells
{
private:
	UnorderedStringMap<SpellInstance> spells;
	const SpellInstance* selectedSpell{ nullptr };

public:
	auto begin() noexcept { return spells.begin(); }
	auto end() noexcept { return spells.end(); }
	auto begin() const noexcept { return spells.begin(); }
	auto end() const noexcept { return spells.end(); }
	auto cbegin() const noexcept { return spells.cbegin(); }
	auto cend() const noexcept { return spells.cend(); }

	auto empty() const noexcept { return spells.empty(); }
	auto size() const noexcept { return spells.size(); }

	auto SelectedSpell() const noexcept { return selectedSpell; }
	void SelectedSpell(const std::string_view id) noexcept;

	bool hasSpell(const std::string_view key) const;

	void addSpell(const std::string_view key, Spell* spell, const Queryable* spellOwner, LevelObjValue spellLevel = 1);

	Spell* getSpell(const std::string_view key) const;

	const SpellInstance* getSpellInstance(const std::string_view key) const;
};
