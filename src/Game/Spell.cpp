#include "Spell.h"
#include "GameUtils.h"
#include "Utils/Utils.h"

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
	case str2int16("id"):
	case str2int16("class"):
	case str2int16("classId"):
		var = Variable(id);
		break;
	case str2int16("name"):
		var = Variable(name);
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
		var = Variable(spellType);
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

void Spell::setDescription(size_t idx, Classifier* classifier, uint16_t skipFirst)
{
	if (idx < descriptionClassifiers.size())
	{
		descriptionClassifiers[idx] = std::make_pair(classifier, skipFirst);
	}
}

bool Spell::getDescription(size_t idx, const Queryable& item, std::string& description) const
{
	if (idx >= descriptionClassifiers.size())
	{
		return false;
	}
	auto classifier = descriptionClassifiers[idx].first;
	if (classifier == nullptr)
	{
		return false;
	}
	description = VarUtils::toString(classifier->get(item, descriptionClassifiers[idx].second));
	if (description.empty() == false)
	{
		description = GameUtils::replaceStringWithQueryable(description, item);
	}
	return true;
}

bool Spell::getTexture(size_t textureNumber, TextureInfo& ti) const
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
			getDescription(i, *this, descriptions[i]);
		}
	}
}
