#pragma once

#include "Game/Properties/LevelObjValue.h"
#include "Game/Spell/SpellInstance.h"
#include "Utils/FixedMap.h"
#include "Utils/Number.h"

class Level;
class PlayerBase;
class PlayerClass;

struct PlayerProperties
{
	mutable std::string name;
	mutable std::array<std::string, 2> descriptions;

	mutable bool updateNameAndDescr{ true };

	uint32_t currentLevel{ 0 };
	uint32_t experience{ 0 };
	uint32_t expNextLevel{ 0 };
	uint32_t points{ 0 };

	LevelObjValue strength{ 0 };
	LevelObjValue strengthItems{ 0 };
	LevelObjValue magic{ 0 };
	LevelObjValue magicItems{ 0 };
	LevelObjValue dexterity{ 0 };
	LevelObjValue dexterityItems{ 0 };
	LevelObjValue vitality{ 0 };
	LevelObjValue vitalityItems{ 0 };

	LevelObjValue life{ 1 };
	LevelObjValue lifeItems{ 0 };
	LevelObjValue lifeDamage{ 0 };
	LevelObjValue mana{ 0 };
	LevelObjValue manaItems{ 0 };
	LevelObjValue manaDamage{ 0 };

	LevelObjValue armor{ 0 };
	LevelObjValue armorItems{ 0 };
	LevelObjValue toArmor{ 0 };
	LevelObjValue toHit{ 0 };
	LevelObjValue toHitItems{ 0 };
	LevelObjValue damageMin{ 0 };
	LevelObjValue damageMax{ 0 };
	LevelObjValue damageMinItems{ 0 };
	LevelObjValue damageMaxItems{ 0 };
	LevelObjValue toDamage{ 0 };

	LevelObjValue resistMagic{ 0 };
	LevelObjValue resistMagicItems{ 0 };
	LevelObjValue resistFire{ 0 };
	LevelObjValue resistFireItems{ 0 };
	LevelObjValue resistLightning{ 0 };
	LevelObjValue resistLightningItems{ 0 };

	FixedMap<uint16_t, Number32, 8> custom;

	bool getIntByHash(const PlayerClass& class_, uint16_t propHash, LevelObjValue& value) const noexcept;

	bool getUIntByHash(uint16_t propHash, uint32_t& value) const noexcept;

	bool setIntByHash(const PlayerClass& class_, uint16_t propHash, LevelObjValue value);

	bool setUIntByHash(PlayerBase& player, uint16_t propHash, uint32_t value, const Level* level);

	bool setNumberByHash(PlayerBase& player, uint16_t propHash, const Number32& value, const Level* level);

	void updateNameAndDescriptions(const PlayerBase& player) const;

	const std::string& Name(const PlayerBase& player) const;

	LevelObjValue StrengthNow() const noexcept { return strength + strengthItems; }
	LevelObjValue MagicNow() const noexcept { return magic + magicItems; }
	LevelObjValue DexterityNow() const noexcept { return dexterity + dexterityItems; }
	LevelObjValue VitalityNow() const noexcept { return vitality + vitalityItems; }

	LevelObjValue LifeNow() const noexcept { return life + lifeItems - lifeDamage; }
	LevelObjValue ManaNow() const noexcept { return mana + manaItems - manaDamage; }

	bool hasMaxStats(const PlayerClass& class_) const noexcept;

	void updateBodyItemValues(PlayerBase& player);

	void updateProperties(PlayerBase& player);
};
