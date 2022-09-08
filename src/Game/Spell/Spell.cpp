#include "Spell.h"
#include "Game/Utils/GameUtils.h"
#include "Utils/StringHash.h"
#include "Utils/Utils.h"

bool Spell::getProperty(const Queryable& spell, const Queryable& player, uint16_t propHash, const std::string_view prop, Variable& var) const
{
	switch (propHash)
	{
	case str2int16("id"):
	case str2int16("class"):
	case str2int16("classId"):
		var = Variable(id);
		break;
	case str2int16("level"):
		var = Variable((int64_t)0);
		break;
	case str2int16("life"):
		var = Variable((int64_t)getLife(spell, player, prop));
		break;
	case str2int16("mana"):
		var = Variable((int64_t)getMana(spell, player, prop));
		break;
	case str2int16("damage"):
		var = Variable((int64_t)getDamage(spell, player, prop));
		break;
	case str2int16("duration"):
		var = Variable((int64_t)getDuration(spell, player, prop));
		break;
	case str2int16("speed"):
		var = Variable((int64_t)getSpeed(spell, player, prop));
		break;
	case str2int16("name"):
		var = Variable(name);
		break;
	case str2int16("d"):
	case str2int16("description"):
	{
		updateDescriptions(spell);
		size_t idx = Utils::strtou(prop);
		if (idx >= descriptions.size())
		{
			idx = 0;
		}
		var = Variable(GameUtils::replaceStringWithQueryable(descriptions[idx], spell));
		break;
	}
	case str2int16("hasDescription"):
	{
		updateDescriptions(spell);
		bool hasDescr = false;
		size_t idx = Utils::strtou(prop);
		if (idx < descriptions.size())
		{
			hasDescr = descriptions[idx].empty() == false;
		}
		var = Variable(hasDescr);
		break;
	}
	case str2int16("type"):
		var = Variable(std::string("spell"));
		break;
	case str2int16("spellType"):
		var = Variable(spellType);
		break;
	case str2int16("propertyCount"):
		var = Variable((int64_t)properties.size());
		break;
	case str2int16("hasProperty"):
		var = Variable(hasIntByHash(str2int16(prop)));
		break;
	default:
	{
		LevelObjValue value;
		if (getIntByHash(propHash, value) == true)
		{
			var = Variable((int64_t)value);
			break;
		}
		else if (evalFormula(propHash, spell, player, value, prop) == true)
		{
			var = Variable((int64_t)value);
			break;
		}
		return false;
	}
	}
	return true;
}

bool Spell::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	return getProperty(*this, *this, propHash, props.second, var);
}

void Spell::setDescription(size_t idx, Classifier* classifier, uint16_t skipFirst)
{
	descriptionClassifiers.set(idx, classifier, skipFirst);
}

bool Spell::getDescription(size_t idx, const Queryable& obj, std::string& description) const
{
	return descriptionClassifiers.getText(idx, obj, description, false);
}

bool Spell::getTexture(uint32_t textureNumber, TextureInfo& ti) const
{
	switch (textureNumber)
	{
	case 0:
	case 1:
		return getTexture1(ti);
	case 2:
		return getTexture2(ti);
	default:
		return false;
	}
}

bool Spell::hasIntByHash(uint16_t propHash) const noexcept
{
	return properties.hasValue(propHash);
}

LevelObjValue Spell::getIntByHash(uint16_t propHash) const
{
	LevelObjValue value = 0;
	getIntByHash(propHash, value);
	return value;
}

bool Spell::getIntByHash(uint16_t propHash, LevelObjValue& value) const
{
	return properties.getValue(propHash, value);
}

bool Spell::getNumberByHash(const Queryable& spell, const Queryable& player,
	uint16_t propHash, const std::string_view minMaxNumber, LevelObjValue& value) const
{
	switch (propHash)
	{
	case str2int16("level"):
		value = 0;
		break;
	case str2int16("life"):
		value = getLife(spell, player, minMaxNumber);
		break;
	case str2int16("mana"):
		value = getMana(spell, player, minMaxNumber);
		break;
	case str2int16("damage"):
		value = getDamage(spell, player, minMaxNumber);
		break;
	case str2int16("duration"):
		value = getDuration(spell, player, minMaxNumber);
		break;
	case str2int16("speed"):
		value = getSpeed(spell, player, minMaxNumber);
		break;
	default:
	{
		if (getIntByHash(propHash, value) == true)
		{
			break;
		}
		else if (evalFormula(propHash, spell, player, value, minMaxNumber) == true)
		{
			break;
		}
		return false;
	}
	}
	return true;
}

bool Spell::evalFormula(uint16_t nameHash, const Queryable& objA,
	const Queryable& objB, LevelObjValue& val, const std::string_view minMaxNumber) const
{
	const auto formula = customFormulas.getValue(nameHash);
	if (formula != nullptr)
	{
		val = (LevelObjValue)formula->eval(objA, objB, minMaxNumber);
		return true;
	}
	return false;
}

bool Spell::getNumberByHash(const Queryable& player, uint16_t propHash, LevelObjValue& value) const
{
	return getNumberByHash(player, propHash, {}, value);
}

bool Spell::getNumberByHash(const Queryable& player, uint16_t propHash, const std::string_view minMaxNumber, LevelObjValue& value) const
{
	return getNumberByHash(*this, player, propHash, minMaxNumber, value);
}

bool Spell::getNumber(const std::string_view prop, Number32& value) const
{
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	LevelObjValue val;
	if (getNumberByHash(*this, propHash, props.second, val) == true)
	{
		value.setInt32(val);
		return true;
	}
	return false;
}

bool Spell::setNumberByHash(uint16_t propHash, const Number32& value)
{
	return setIntByHash(propHash, value.getInt32());
}

bool Spell::setIntByHash(uint16_t propHash, LevelObjValue value)
{
	if (properties.setValue(propHash, value) == false)
	{
		return false;
	}
	updateDescr = true;
	return true;
}

void Spell::updateDescriptions(const Queryable& spellObj) const
{
	if (updateDescr == true)
	{
		updateDescr = false;
		for (size_t i = 0; i < descriptions.size(); i++)
		{
			getDescription(i, spellObj, descriptions[i]);
		}
	}
}

void Spell::setFormula(uint16_t nameHash, const std::string_view formula)
{
	if (nameHash == str2int16(""))
	{
		return;
	}
	switch (nameHash)
	{
	case str2int16("life"):
	{
		formulas[0] = formula;
		break;
	}
	case str2int16("mana"):
	{
		formulas[1] = formula;
		break;
	}
	case str2int16("damage"):
	{
		formulas[2] = formula;
		break;
	}
	case str2int16("duration"):
	{
		formulas[3] = formula;
		break;
	}
	case str2int16("speed"):
	{
		formulas[4] = formula;
		break;
	}
	default:
		customFormulas.setValue(nameHash, formula);
		break;
	}
}

void Spell::deleteFormula(uint16_t nameHash)
{
	setFormula(nameHash, {});
}

LevelObjValue Spell::getLife(const Queryable& spell,
	const Queryable& player, const std::string_view minMaxNumber) const
{
	return formulas.eval<LevelObjValue>(LifeFormula, spell, player, minMaxNumber);
}

LevelObjValue Spell::getMana(const Queryable& spell,
	const Queryable& player, const std::string_view minMaxNumber) const
{
	return formulas.eval<LevelObjValue>(ManaFormula, spell, player, minMaxNumber);
}

LevelObjValue Spell::getDamage(const Queryable& spell,
	const Queryable& player, const std::string_view minMaxNumber) const
{
	return formulas.eval<LevelObjValue>(DamageFormula, spell, player, minMaxNumber);
}

LevelObjValue Spell::getDuration(const Queryable& spell,
	const Queryable& player, const std::string_view minMaxNumber) const
{
	return formulas.eval<LevelObjValue>(DurationFormula, spell, player, minMaxNumber);
}

LevelObjValue Spell::getSpeed(const Queryable& spell,
	const Queryable& player, const std::string_view minMaxNumber) const
{
	return formulas.eval<LevelObjValue>(SpeedFormula, spell, player, minMaxNumber);
}
