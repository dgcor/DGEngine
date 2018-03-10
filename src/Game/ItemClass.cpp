#include "ItemClass.h"
#include <algorithm>
#include "Game.h"
#include "GameUtils.h"

ItemClass::ItemClass(const std::shared_ptr<TexturePack>& textureDrop_,
	const std::shared_ptr<TexturePack>& textureInventory_,
	size_t inventoryIdx_) : textureDrop(textureDrop_),
	textureInventory(textureInventory_), inventoryIdx(inventoryIdx_)
{
	if (textureDrop_ != nullptr)
	{
		dropTextureIndexRange.first = 0;
		dropTextureIndexRange.second = textureDrop_->size();
	}
}

void ItemClass::setDropTexturePack(const std::shared_ptr<TexturePack>& textureDrop_) noexcept
{
	textureDrop = textureDrop_;
	if (textureDrop_ != nullptr)
	{
		dropTextureIndexRange.first = 0;
		dropTextureIndexRange.second = textureDrop_->size();
	}
}

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

LevelObjValue ItemClass::getDefaultByHash(uint16_t propHash) const noexcept
{
	LevelObjValue value = 0;
	getDefaultByHash(propHash, value);
	return value;
}

LevelObjValue ItemClass::getDefault(const char* prop) const noexcept
{
	LevelObjValue value = 0;
	getDefault(prop, value);
	return value;
}

bool ItemClass::getDefault(const char* prop, LevelObjValue& value) const noexcept
{
	if (defaults.empty() == false)
	{
		return getDefaultByHash(str2int16(prop), value);
	}
	return false;
}

bool ItemClass::getDefaultByHash(uint16_t propHash, LevelObjValue& value) const noexcept
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

void ItemClass::setDescription(size_t idx, Namer* namer, uint16_t skipFirst)
{
	if (idx < descriptions.size())
	{
		descriptions[idx] = std::make_pair(namer, skipFirst);
	}
}

bool ItemClass::getDescription(size_t idx, const Queryable& item, std::string& description) const
{
	if (idx >= descriptions.size())
	{
		return false;
	}
	auto namer = descriptions[idx].first;
	if (namer == nullptr)
	{
		return false;
	}
	description = namer->getName(item, descriptions[idx].second);
	if (description.empty() == false)
	{
		description = GameUtils::replaceStringWithQueryable(description, item);
	}
	return true;
}

void ItemClass::setFormula(uint16_t nameHash, const Formula& formula)
{
	if (nameHash == str2int16(""))
	{
		return;
	}
	size_t i = 0;
	for (; i < formulasSize; i++)
	{
		auto& elem = formulas[i];
		if (elem.first == nameHash)
		{
			elem.second = formula;
			return;
		}
	}
	if (i < formulas.size())
	{
		formulas[i] = std::make_pair(nameHash, formula);
		formulasSize++;
	}
}

void ItemClass::deleteFormula(uint16_t nameHash)
{
	if (nameHash == str2int16(""))
	{
		return;
	}
	for (size_t i = 0; i < formulasSize; i++)
	{
		if (formulas[i].first == nameHash)
		{
			formulas[i].second = {};
			std::move(formulas.begin() + i + 1,
				formulas.begin() + formulasSize,
				formulas.begin() + i);
			formulasSize--;
			return;
		}
	}
}

bool ItemClass::evalFormula(uint16_t nameHash, const LevelObject& obj, LevelObjValue& val) const
{
	return evalFormula(nameHash, obj, obj, val);
}

bool ItemClass::evalFormula(uint16_t nameHash, const LevelObject& objA,
	const LevelObject& objB, LevelObjValue& val) const
{
	for (size_t i = 0; i < formulasSize; i++)
	{
		const auto& formula = formulas[i];
		if (formula.first == nameHash)
		{
			val = (LevelObjValue)formula.second.eval(objA, objB);
			return true;
		}
	}
	return false;
}

LevelObjValue ItemClass::evalFormula(uint16_t nameHash, const LevelObject& obj) const
{
	return evalFormula(nameHash, obj, obj);
}

LevelObjValue ItemClass::evalFormula(uint16_t nameHash, const LevelObject& objA,
	const LevelObject& objB) const
{
	LevelObjValue val = 0;
	evalFormula(nameHash, objA, objB, val);
	return val;
}
