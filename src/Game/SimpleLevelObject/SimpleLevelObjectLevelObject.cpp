#include "SimpleLevelObjectLevelObject.h"
#include "Game/Formula.h"
#include "SimpleLevelObject.h"
#include "Utils/Utils.h"

bool SimpleLevelObjectLevelObject::getNumber(const SimpleLevelObject& levelObj, const std::string_view prop, Number32& value)
{
	LevelObjValue val;
	bool ret = levelObj.getIntByHash(str2int16(prop), val);
	if (ret == true)
	{
		value.setInt32(val);
	}
	return ret;
}

bool SimpleLevelObjectLevelObject::getProperty(const SimpleLevelObject& levelObj, const std::string_view prop, Variable& var)
{
	if (prop.empty() == true)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	if (levelObj.getLevelObjProp(propHash, props.second, var) == true)
	{
		return true;
	}
	switch (propHash)
	{
	case str2int16("name"):
		var = Variable(levelObj.Name());
		break;
	case str2int16("simpleName"):
		var = Variable(levelObj.SimpleName());
		break;
	case str2int16("text1"):
		var = Variable(levelObj.text1);
		break;
	case str2int16("text2"):
		var = Variable(levelObj.text2);
		break;
	case str2int16("d"):
	case str2int16("description"):
	{
		levelObj.updateNameAndDescriptions();
		size_t idx = Utils::strtou(props.second);
		if (idx >= levelObj.descriptions.size())
		{
			idx = 0;
		}
		var = Variable(levelObj.descriptions[idx]);
		break;
	}
	case str2int16("eval"):
		var = Variable((int64_t)Formula::evalString(props.second, levelObj));
		break;
	case str2int16("evalMin"):
		var = Variable((int64_t)Formula::evalMinString(props.second, levelObj));
		break;
	case str2int16("evalMax"):
		var = Variable((int64_t)Formula::evalMaxString(props.second, levelObj));
		break;
	case str2int16("evalf"):
		var = Variable(Formula::evalString(props.second, levelObj));
		break;
	case str2int16("evalMinf"):
		var = Variable(Formula::evalMinString(props.second, levelObj));
		break;
	case str2int16("evalMaxf"):
		var = Variable(Formula::evalMaxString(props.second, levelObj));
		break;
	default:
	{
		LevelObjValue value;
		if (levelObj.getIntByHash(propHash, value) == true)
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
