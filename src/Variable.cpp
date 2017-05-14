#include "Variable.h"
#include "Utils.h"

namespace VarUtils
{
	bool toBool(const Variable& var)
	{
		if (var.is<bool>())
		{
			return var.get<bool>();
		}
		else if (var.is<int64_t>())
		{
			return var.get<int64_t>() != 0;
		}
		else if (var.is<double>())
		{
			return var.get<double>() != 0.0;
		}
		return var.get<std::string>().empty() == false;
	}

	double toDouble(const Variable& var)
	{
		if (var.is<double>())
		{
			return var.get<double>();
		}
		else if (var.is<int64_t>())
		{
			return (double)var.get<int64_t>();
		}
		else if (var.is<bool>())
		{
			return (var.get<bool>() ? 1.0 : 0.0);
		}
		return std::atof(var.get<std::string>().c_str());
	}

	int64_t toLong(const Variable& var)
	{
		if (var.is<int64_t>())
		{
			return var.get<int64_t>();
		}
		else if (var.is<double>())
		{
			return (int64_t)var.get<double>();
		}
		else if (var.is<bool>())
		{
			return (var.get<bool>() ? 1 : 0);
		}
		return std::atol(var.get<std::string>().c_str());
	}

	std::string toString(const Variable& var)
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
			return Utils::toString(var.get<double>());
		}
		// bool
		if (var.get<bool>() == true)
		{
			return { "true" };
		}
		else
		{
			return { "false" };
		}
	}
}
