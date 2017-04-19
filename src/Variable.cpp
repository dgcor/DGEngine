#include "Variable.h"

namespace VarUtils
{
	std::string toString(const Variable& var, const std::string& defVal)
	{
		if (var.is<std::string>())
		{
			return var.get<std::string>();
		}
		else if (var.is<int64_t>())
		{
			return std::to_string(var.get<int64_t>());
		}
		else if (var.is<double>())
		{
			return std::to_string(var.get<double>());
		}
		else if (var.is<bool>())
		{
			if (var.get<bool>() == true)
			{
				return { "true" };
			}
			else
			{
				return { "false" };
			}
		}
		return defVal;
	}
}
