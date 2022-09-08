#include "LevelObjectQueryable.h"
#include "Utils/StringHash.h"

bool LevelObjectQueryable::setProperty(const std::string_view prop, const Variable& var)
{
	if (prop.empty() == true)
	{
		return false;
	}
	Number32 value;
	if (std::holds_alternative<bool>(var) == true)
	{
		value.setInt32(std::get<bool>(var) ? 1 : 0);
	}
	else if (std::holds_alternative<int64_t>(var) == true)
	{
		value.setInt32((LevelObjValue)std::get<int64_t>(var));
	}
	else if (std::holds_alternative<double>(var) == true)
	{
		value.setFloat((float)std::get<double>(var));
	}
	return setNumberByHash(str2int16(prop), value);
}
