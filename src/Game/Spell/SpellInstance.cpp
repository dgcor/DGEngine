#include "SpellInstance.h"
#include "Utils/StringHash.h"
#include "Utils/Utils.h"

bool SpellInstance::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	switch (propHash)
	{
	case str2int16("level"):
		var = Variable((int64_t)spellLevel);
		break;
	default:
		return spell->getProperty(*this, *spellOwner, propHash, props.second, var);
	}
	return true;
}

bool SpellInstance::getTexture(uint32_t textureNumber, TextureInfo& ti) const
{
	if (spell != nullptr)
	{
		return spell->getTexture(textureNumber, ti);
	}
	return false;
}

bool SpellInstance::getNumberByHash(uint16_t propHash, LevelObjValue& value) const
{
	return getNumberByHash(*spellOwner, propHash, {}, value);
}

bool SpellInstance::getNumberByHash(const Queryable& player, uint16_t propHash, LevelObjValue& value) const
{
	return getNumberByHash(player, propHash, {}, value);
}

bool SpellInstance::getNumberByHash(uint16_t propHash, const std::string_view minMaxNumber, LevelObjValue& value) const
{
	return getNumberByHash(*spellOwner, propHash, minMaxNumber, value);
}

bool SpellInstance::getNumberByHash(const Queryable& player, uint16_t propHash, const std::string_view minMaxNumber, LevelObjValue& value) const
{
	if (propHash == str2int16("level"))
	{
		value = spellLevel;
		return true;
	}
	else if (spell != nullptr)
	{
		return spell->getNumberByHash(*this, player, propHash, minMaxNumber, value);
	}
	return false;
}

bool SpellInstance::getNumber(const std::string_view prop, Number32& value) const
{
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	LevelObjValue val;
	if (getNumberByHash(*spellOwner, propHash, props.second, val) == true)
	{
		value.setInt32(val);
		return true;
	}
	return false;
}
