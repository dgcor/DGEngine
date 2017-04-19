#include "ItemClass.h"
#include "GameUtils.h"

void ItemClass::setDefault(const char* prop, LevelObjValue val)
{
	auto propertyHash = str2int16(prop);
	if (propertyHash == str2int16(""))
	{
		return;
	}
	for (auto& elem : defaults)
	{
		if (elem.first == propertyHash)
		{
			elem.second = val;
			return;
		}
	}
	defaults.push_back(std::make_pair(propertyHash, val));
}

LevelObjValue ItemClass::getDefaultByHash(uint16_t propHash) const
{
	LevelObjValue value = 0;
	getDefaultByHash(propHash, value);
	return value;
}

LevelObjValue ItemClass::getDefault(const char* prop) const
{
	LevelObjValue value = 0;
	getDefault(prop, value);
	return value;
}

bool ItemClass::getDefault(const char* prop, LevelObjValue& value) const
{
	if (defaults.empty() == false)
	{
		return getDefaultByHash(str2int16(prop), value);
	}
	return false;
}

bool ItemClass::getDefaultByHash(uint16_t propHash, LevelObjValue& value) const
{
	for (const auto& prop : defaults)
	{
		if (prop.first == propHash)
		{
			value = prop.second;
			return true;
		}
	}
	return false;
}

bool ItemClass::getFullName(const Queryable& item, std::string& fullName) const
{
	std::string strPrefix;
	if (prefix != nullptr)
	{
		strPrefix = prefix->getName(item);
	}
	std::string strSuffix;
	if (suffix != nullptr)
	{
		strSuffix = suffix->getName(item);
	}
	bool hasPrefix = strPrefix.empty() == false;
	bool hasSuffix = strSuffix.empty() == false;

	if (hasPrefix == false && hasSuffix == false)
	{
		return false;
	}
	if (hasPrefix == true)
	{
		fullName = GameUtils::replaceStringWithQueryable(strPrefix, item) + ' ';
	}
	else
	{
		fullName.clear();
	}
	if (shortName.empty() == false)
	{
		fullName += shortName;
	}
	else
	{
		fullName += name;
	}
	if (hasSuffix == true)
	{
		fullName += ' ' + GameUtils::replaceStringWithQueryable(strSuffix, item);
	}
	return true;
}

void ItemClass::setDescription(size_t idx, const std::shared_ptr<Namer>& namer)
{
	if (idx < descriptions.size())
	{
		descriptions[idx] = namer;
	}
}

bool ItemClass::getDescription(size_t idx, const Queryable& item, std::string& description) const
{
	if (idx >= descriptions.size())
	{
		return false;
	}
	auto namer = descriptions[idx].get();
	if (namer == nullptr)
	{
		return false;
	}
	description = namer->getName(item);
	if (description.empty() == false)
	{
		description = GameUtils::replaceStringWithQueryable(description, item);
	}
	return true;
}
