#include "Queryable.h"

bool Queryable::getNumber(const std::string_view prop, Number32& value) const
{
	Variable var;
	if (getProperty(prop, var) == true)
	{
		if (std::holds_alternative<int64_t>(var) == true)
		{
			value.setInt32((int32_t)std::get<int64_t>(var));
			return true;
		}
		else if (std::holds_alternative<double>(var) == true)
		{
			value.setFloat((float)std::get<double>(var));
			return true;
		}
		else if (std::holds_alternative<bool>(var) == true)
		{
			value.setInt32(std::get<bool>(var) ? 1 : 0);
			return true;
		}
	}
	return false;
}
