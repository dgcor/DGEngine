#include "ItemClass.h"
#include "GameUtils.h"

void ItemClass::setDefault(const char* prop, int16_t val)
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
		fullName = strPrefix + ' ';
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
		fullName += ' ' + strSuffix;
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
	std::string str = namer->getName(item);
	if (str.empty() == true)
	{
		return false;
	}
	description = GameUtils::replaceStringWithQueryable(str, item);
	return true;
}
