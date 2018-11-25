#include "Spell.h"
#include "Utils/Utils.h"

Spell::Spell(const SpellClass* class__) : class_(class__)
{
	applyDefaults();
}

bool Spell::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	switch (propHash)
	{
	case str2int16("class"):
	case str2int16("classId"):
		var = Variable(class_->Id());
		break;
	case str2int16("name"):
		var = Variable(Name());
		break;
	case str2int16("d"):
	case str2int16("description"):
	{
		updateDescriptions();
		size_t idx = Utils::strtou(props.second);
		if (idx >= descriptions.size())
		{
			idx = 0;
		}
		var = Variable(descriptions[idx]);
		break;
	}
	case str2int16("hasDescription"):
	{
		updateDescriptions();
		bool hasDescr = false;
		size_t idx = Utils::strtou(props.second);
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
		var = Variable(SpellType());
		break;
	case str2int16("propertyCount"):
		var = Variable((int64_t)properties.size());
		break;
	case str2int16("hasProperty"):
		var = Variable(hasInt(props.second));
		break;
	default:
	{
		LevelObjValue value;
		if (getIntByHash(propHash, value) == true)
		{
			var = Variable((int64_t)value);
			break;
		}
		else
		{
			return false;
		}
	}
	}
	return true;
}

bool Spell::getTexture(size_t textureNumber, TextureInfo& ti) const
{
	switch (textureNumber)
	{
	case 0:
	case 1:
		return class_->getTexture1(ti);
	case 2:
		return class_->getTexture2(ti);
	default:
		return false;
	}
}

bool Spell::hasIntByHash(uint16_t propHash) const noexcept
{
	return properties.hasValue(propHash);
}

bool Spell::hasInt(const std::string_view prop) const noexcept
{
	return hasIntByHash(str2int16(prop));
}

LevelObjValue Spell::getIntByHash(uint16_t propHash) const
{
	LevelObjValue value = 0;
	getIntByHash(propHash, value);
	return value;
}

LevelObjValue Spell::getInt(const std::string_view prop) const
{
	LevelObjValue value = 0;
	getInt(prop, value);
	return value;
}

bool Spell::getIntByHash(uint16_t propHash, LevelObjValue& value) const
{
	return properties.getValue(propHash, value);
}

bool Spell::getInt(const std::string_view prop, LevelObjValue& value) const
{
	return getIntByHash(str2int16(prop), value);
}

void Spell::setIntByHash(uint16_t propHash, LevelObjValue value)
{
	if (properties.setValue(propHash, value) == false)
	{
		return;
	}
	updateDescr = true;
}

void Spell::setInt(const std::string_view prop, LevelObjValue value)
{
	setIntByHash(str2int16(prop), value);
}

void Spell::updateDescriptions() const
{
	if (updateDescr == true)
	{
		updateDescr = false;
		for (size_t i = 0; i < descriptions.size(); i++)
		{
			class_->getDescription(i, *this, descriptions[i]);
		}
	}
}

void Spell::applyDefaults()
{
	for (const auto& prop : class_->Defaults())
	{
		setIntByHash(prop.first, prop.second);
	}
}
