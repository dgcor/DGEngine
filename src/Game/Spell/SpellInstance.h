#pragma once

#include "Spell.h"

struct SpellInstance : public LevelObjectQueryable
{
	Spell* spell{ nullptr };
	const Queryable* spellOwner{ nullptr };
	LevelObjValue spellLevel{ 0 };

	SpellInstance() {}
	SpellInstance(Spell* spell_, const Queryable* spellOwner_, LevelObjValue spellLevel_) :
		spell(spell_), spellOwner(spellOwner_), spellLevel(spellLevel_) {}

	bool getNumberByHash(uint16_t propHash, LevelObjValue& value) const;
	bool getNumberByHash(const Queryable& player, uint16_t propHash, LevelObjValue& value) const override;
	bool getNumberByHash(uint16_t propHash, const std::string_view minMaxNumber, LevelObjValue& value) const;
	bool getNumberByHash(const Queryable& player, uint16_t propHash, const std::string_view minMaxNumber, LevelObjValue& value) const;

	bool getNumber(const std::string_view prop, Number32& value) const override;
	bool getProperty(const std::string_view prop, Variable& var) const override;
	bool getTexture(uint32_t textureNumber, TextureInfo& ti) const override;
};
