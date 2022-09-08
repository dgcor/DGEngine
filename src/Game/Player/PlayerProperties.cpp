#include "PlayerProperties.h"
#include <algorithm>
#include "Game/GameHashes.h"
#include "PlayerBase.h"

bool PlayerProperties::getIntByHash(const PlayerClass& class_, uint16_t propHash, LevelObjValue& value) const noexcept
{
	switch (propHash)
	{
	case str2int16("strength"):
		value = strength;
		break;
	case str2int16("strengthItems"):
		value = strengthItems;
		break;
	case str2int16("strengthNow"):
		value = StrengthNow();
		break;
	case str2int16("magic"):
		value = magic;
		break;
	case str2int16("magicItems"):
		value = magicItems;
		break;
	case str2int16("magicNow"):
		value = MagicNow();
		break;
	case str2int16("dexterity"):
		value = dexterity;
		break;
	case str2int16("dexterityItems"):
		value = dexterityItems;
		break;
	case str2int16("dexterityNow"):
		value = DexterityNow();
		break;
	case str2int16("vitality"):
		value = vitality;
		break;
	case str2int16("vitalityItems"):
		value = vitalityItems;
		break;
	case str2int16("vitalityNow"):
		value = VitalityNow();
		break;
	case str2int16("life"):
		value = life;
		break;
	case str2int16("lifeItems"):
		value = lifeItems;
		break;
	case str2int16("lifeDamage"):
		value = lifeDamage;
		break;
	case str2int16("lifeNow"):
		value = LifeNow();
		break;
	case str2int16("mana"):
		value = mana;
		break;
	case str2int16("manaItems"):
		value = manaItems;
		break;
	case str2int16("manaDamage"):
		value = manaDamage;
		break;
	case str2int16("manaNow"):
		value = ManaNow();
		break;
	case str2int16("armor"):
		value = armor;
		break;
	case str2int16("armorItems"):
		value = armorItems;
		break;
	case str2int16("toArmor"):
		value = toArmor;
		break;
	case str2int16("toHit"):
		value = toHit;
		break;
	case str2int16("toHitItems"):
		value = toHitItems;
		break;
	case str2int16("damageMin"):
		value = damageMin;
		break;
	case str2int16("damageMinItems"):
		value = damageMinItems;
		break;
	case str2int16("damageMax"):
		value = damageMax;
		break;
	case str2int16("damageMaxItems"):
		value = damageMaxItems;
		break;
	case str2int16("toDamage"):
		value = toDamage;
		break;
	case str2int16("resistMagic"):
		value = resistMagic;
		break;
	case str2int16("resistMagicItems"):
		value = resistMagicItems;
		break;
	case str2int16("resistFire"):
		value = resistFire;
		break;
	case str2int16("resistFireItems"):
		value = resistFireItems;
		break;
	case str2int16("resistLightning"):
		value = resistLightning;
		break;
	case str2int16("resistLightningItems"):
		value = resistLightningItems;
		break;
	case str2int16("maxStrength"):
		value = class_.MaxStrength();
		break;
	case str2int16("maxMagic"):
		value = class_.MaxMagic();
		break;
	case str2int16("maxDexterity"):
		value = class_.MaxDexterity();
		break;
	case str2int16("maxVitality"):
		value = class_.MaxVitality();
		break;
	case str2int16("maxResistMagic"):
		value = class_.MaxResistMagic();
		break;
	case str2int16("maxResistFire"):
		value = class_.MaxResistFire();
		break;
	case str2int16("maxResistLightning"):
		value = class_.MaxResistLightning();
		break;
	default:
		return false;
	}
	return true;
}

bool PlayerProperties::getUIntByHash(uint16_t propHash, uint32_t& value) const noexcept
{
	switch (propHash)
	{
	case str2int16("level"):
		value = currentLevel;
		break;
	case str2int16("experience"):
		value = experience;
		break;
	case str2int16("expNextLevel"):
		value = expNextLevel;
		break;
	case str2int16("points"):
		value = points;
		break;
	default:
		return false;
	}
	return true;
}

bool PlayerProperties::setIntByHash(const PlayerClass& class_, uint16_t propHash, LevelObjValue value)
{
	switch (propHash)
	{
	case str2int16("strength"):
		strength = std::clamp(value, 0, class_.MaxStrength());
		break;
	case str2int16("magic"):
		magic = std::clamp(value, 0, class_.MaxMagic());
		break;
	case str2int16("dexterity"):
		dexterity = std::clamp(value, 0, class_.MaxDexterity());
		break;
	case str2int16("vitality"):
		vitality = std::clamp(value, 0, class_.MaxVitality());
		break;
	case str2int16("lifeDamage"):
		lifeDamage = std::max(value, 0);
		break;
	case str2int16("manaDamage"):
		manaDamage = std::max(value, 0);
		break;
	default:
		return false;
	}
	updateNameAndDescr = true;
	return true;
}

bool PlayerProperties::setUIntByHash(PlayerBase& player, uint16_t propHash, uint32_t value, const Level* level)
{
	switch (propHash)
	{
	case str2int16("experience"):
	{
		experience = value;
		if (level != nullptr)
		{
			player.updateLevelFromExperience(*level, true);
		}
		break;
	}
	case str2int16("points"):
		points = value;
		break;
	default:
		return false;
	}
	updateNameAndDescr = true;
	return true;
}

bool PlayerProperties::setNumberByHash(PlayerBase& player, uint16_t propHash, const Number32& value, const Level* level)
{
	if (setIntByHash(*player.Class(), propHash, value.getInt32()) == true)
	{
		return true;
	}
	if (setUIntByHash(player, propHash, value.getUInt32(), level) == true)
	{
		return true;
	}
	switch (propHash)
	{
	case str2int16(""):
	case str2int16("hasMaxStats"):
	case str2int16("level"):
	case str2int16("expNextLevel"):
	case str2int16("strengthItems"):
	case str2int16("strengthNow"):
	case str2int16("magicItems"):
	case str2int16("magicNow"):
	case str2int16("dexterityItems"):
	case str2int16("dexterityNow"):
	case str2int16("vitalityItems"):
	case str2int16("vitalityNow"):
	case str2int16("life"):
	case str2int16("lifeItems"):
	case str2int16("lifeNow"):
	case str2int16("mana"):
	case str2int16("manaItems"):
	case str2int16("manaNow"):
	case str2int16("armor"):
	case str2int16("armorItems"):
	case str2int16("toArmor"):
	case str2int16("toHit"):
	case str2int16("toHitItems"):
	case str2int16("damageMin"):
	case str2int16("damageMinItems"):
	case str2int16("damageMax"):
	case str2int16("damageMaxItems"):
	case str2int16("toDamage"):
	case str2int16("resistMagic"):
	case str2int16("resistFire"):
	case str2int16("resistLightning"):
	case str2int16("maxStrength"):
	case str2int16("maxMagic"):
	case str2int16("maxDexterity"):
	case str2int16("maxVitality"):
	case str2int16("maxResistMagic"):
	case str2int16("maxResistFire"):
	case str2int16("maxResistLightning"):
		return false;
	default:
	{
		updateNameAndDescr = true;
		return custom.setValue(propHash, value);
	}
	}
}

void PlayerProperties::updateNameAndDescriptions(const PlayerBase& player) const
{
	if (updateNameAndDescr == true)
	{
		updateNameAndDescr = false;
		if (player.Class()->getFullName(player, name) == false &&
			name.empty() == true)
		{
			name = player.Class()->Name();
		}
		for (size_t i = 0; i < descriptions.size(); i++)
		{
			player.Class()->getDescription(i, player, descriptions[i]);
		}
	}
}

const std::string& PlayerProperties::Name(const PlayerBase& player) const
{
	updateNameAndDescriptions(player);
	return name;
}

bool PlayerProperties::hasMaxStats(const PlayerClass& class_) const noexcept
{
	return (strength >= class_.MaxStrength() &&
		magic >= class_.MaxMagic() &&
		dexterity >= class_.MaxDexterity() &&
		vitality >= class_.MaxVitality());
}

void PlayerProperties::updateBodyItemValues(PlayerBase& player)
{
	strengthItems = 0;
	magicItems = 0;
	dexterityItems = 0;
	vitalityItems = 0;
	lifeItems = 0;
	manaItems = 0;
	armor = 0;
	armorItems = 0;
	toArmor = 0;
	toHitItems = 0;
	damageMin = 0;
	damageMax = 0;
	damageMinItems = 0;
	damageMaxItems = 0;
	toDamage = 0;
	resistMagicItems = 0;
	resistFireItems = 0;
	resistLightningItems = 0;
	player.setLightSource(player.getDefaultLightSource());
	LevelObjValue toLightRadius = 0;
	LevelObjValue singleItemDamage = 0;

	if (player.hasBodyInventory() == false)
	{
		return;
	}
	for (const auto& item : player.getInventory(player.getBodyInventoryIdx()))
	{
		if (item.Identified() == true)
		{
			auto allAttributes = item.getIntByHash(ItemProp::AllAttributes);
			strengthItems += allAttributes + item.getIntByHash(ItemProp::Strength);
			magicItems += allAttributes + item.getIntByHash(ItemProp::Magic);
			dexterityItems += allAttributes + item.getIntByHash(ItemProp::Dexterity);
			vitalityItems += allAttributes + item.getIntByHash(ItemProp::Vitality);
		}
		if (player.canUseObject(item) == false)
		{
			continue;
		}
		if (item.Identified() == true)
		{
			lifeItems += item.getIntByHash(ItemProp::Life);
			manaItems += item.getIntByHash(ItemProp::Mana);

			auto resistAll = item.getIntByHash(ItemProp::ResistAll);
			resistMagicItems += resistAll + item.getIntByHash(ItemProp::ResistMagic);
			resistFireItems += resistAll + item.getIntByHash(ItemProp::ResistFire);
			resistLightningItems += resistAll + item.getIntByHash(ItemProp::ResistLightning);

			toArmor += item.getIntByHash(ItemProp::ToArmor);
			toDamage += item.getIntByHash(ItemProp::ToDamage);

			auto damage = item.getIntByHash(ItemProp::Damage);
			damageMin += damage;
			damageMax += damage;
		}
		armorItems += item.getIntByHash(ItemProp::Armor);
		toHitItems += item.getIntByHash(ItemProp::ToHit);
		damageMinItems += item.getIntByHash(ItemProp::DamageMin);
		damageMaxItems += item.getIntByHash(ItemProp::DamageMax);

		toLightRadius += item.getIntByHash(ItemProp::ToLightRadius);

		if (damageMax <= 0)
		{
			singleItemDamage = std::max(singleItemDamage, item.getIntByHash(ItemProp::SingleItemDamage));
		}
	}
	auto toArmorPercentage = (float)toArmor * 0.01f;
	armor += (LevelObjValue)(armorItems * toArmorPercentage);

	auto toDamagePercentage = 1.f + ((float)toDamage * 0.01f);
	if (damageMinItems > 0)
	{
		damageMin += (LevelObjValue)(damageMinItems * toDamagePercentage);
	}
	else if (damageMin <= 0)
	{
		Number32 defaultDamage;
		if (custom.getValue(ItemProp::DefaultDamageMin, defaultDamage))
		{
			damageMin = std::max(0, defaultDamage.getInt32());
		}
		else
		{
			damageMin = 0;
		}
	}
	if (damageMaxItems > 0)
	{
		damageMax += (LevelObjValue)(damageMaxItems * toDamagePercentage);
	}
	else if (damageMax <= 0)
	{
		Number32 defaultDamage;
		if (custom.getValue(ItemProp::DefaultDamageMax, defaultDamage))
		{
			damageMax = defaultDamage.getInt32();
		}
		else
		{
			damageMax = damageMin;
		}
		if (singleItemDamage > 0)
		{
			damageMax += singleItemDamage;
		}
	}
	if (damageMin > damageMax)
	{
		damageMax = damageMin;
	}
	if (toLightRadius != 0)
	{
		auto radiusPercentage = (float)std::clamp(toLightRadius, -80, 50) * 0.01f;
		auto newRadius = (float)player.getLightRadius();
		newRadius = std::round(newRadius + (newRadius * radiusPercentage));
		player.setLightRadius((uint8_t)std::clamp(newRadius, 1.f, 255.f));
	}
}

void PlayerProperties::updateProperties(PlayerBase& player)
{
	updateBodyItemValues(player);

	auto class_ = player.Class();

	life = class_->getActualLife(player, life);
	mana = class_->getActualMana(player, mana);
	armor += class_->getActualArmor(player, 0);
	toHit = class_->getActualToHit(player, toHit);

	resistMagic = class_->getActualResistMagic(player, resistMagicItems);
	resistMagic = std::clamp(resistMagic, 0, class_->MaxResistMagic());
	resistFire = class_->getActualResistFire(player, resistFireItems);
	resistFire = std::clamp(resistFire, 0, class_->MaxResistFire());
	resistLightning = class_->getActualResistLightning(player, resistLightningItems);
	resistLightning = std::clamp(resistLightning, 0, class_->MaxResistLightning());

	auto damage = class_->getActualDamage(player, 0);
	damageMin += damage;
	damageMax += damage;
}
