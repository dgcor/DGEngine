#include "Variable.h"
#include "Utils.h"

namespace VarUtils
{
	bool toBool(const Variable& var)
	{
		if (std::holds_alternative<bool>(var))
		{
			return std::get<bool>(var);
		}
		else if (std::holds_alternative<int64_t>(var))
		{
			return std::get<int64_t>(var) != 0;
		}
		else if (std::holds_alternative<double>(var))
		{
			return std::get<double>(var) != 0.0;
		}
		return std::get<std::string>(var).empty() == false;
	}

	double toDouble(const Variable& var)
	{
		if (std::holds_alternative<double>(var))
		{
			return std::get<double>(var);
		}
		else if (std::holds_alternative<int64_t>(var))
		{
			return (double)std::get<int64_t>(var);
		}
		else if (std::holds_alternative<bool>(var))
		{
			return (std::get<bool>(var) ? 1.0 : 0.0);
		}
		return std::atof(std::get<std::string>(var).c_str());
	}

	int64_t toLong(const Variable& var)
	{
		if (std::holds_alternative<int64_t>(var))
		{
			return std::get<int64_t>(var);
		}
		else if (std::holds_alternative<double>(var))
		{
			return (int64_t)std::get<double>(var);
		}
		else if (std::holds_alternative<bool>(var))
		{
			return (std::get<bool>(var) ? 1 : 0);
		}
		return std::atol(std::get<std::string>(var).c_str());
	}

	std::string toString(const Variable& var)
	{
		if (std::holds_alternative<std::string>(var))
		{
			return std::get<std::string>(var);
		}
		else if (std::holds_alternative<int64_t>(var))
		{
			return std::to_string(std::get<int64_t>(var));
		}
		else if (std::holds_alternative<double>(var))
		{
			return Utils::toString(std::get<double>(var));
		}
		// bool
		if (std::get<bool>(var) == true)
		{
			return { "true" };
		}
		else
		{
			return { "false" };
		}
	}
}
