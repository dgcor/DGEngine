#include "VarUtils.h"
#include "Utils/Utils.h"

namespace VarUtils
{
	bool toBool(const Variable& var) noexcept
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
		else if (std::holds_alternative<std::string>(var))
		{
			return std::get<std::string>(var).empty() == false;
		}
		return false;
	}

	double toDouble(const Variable& var) noexcept
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
		else if (std::holds_alternative<std::string>(var))
		{
			return Utils::strtod(std::get<std::string>(var));
		}
		return 0.0;
	}

	int64_t toInt64(const Variable& var) noexcept
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
		else if (std::holds_alternative<std::string>(var))
		{
			return Utils::strtoll(std::get<std::string>(var));
		}
		return 0;
	}

	std::string toString(const Variable& var)
	{
		if (std::holds_alternative<std::string>(var))
		{
			return std::get<std::string>(var);
		}
		else if (std::holds_alternative<int64_t>(var))
		{
			return Utils::toString(std::get<int64_t>(var));
		}
		else if (std::holds_alternative<double>(var))
		{
			return Utils::toString(std::get<double>(var));
		}
		else if (std::holds_alternative<bool>(var))
		{
			if (std::get<bool>(var) == true)
			{
				return { "true" };
			}
			else
			{
				return { "false" };
			}
		}
		else if (std::holds_alternative<Int64Tuple2>(var))
		{
			return "[" +
				Utils::toString(std::get<0>(std::get<Int64Tuple2>(var))) + ", " +
				Utils::toString(std::get<1>(std::get<Int64Tuple2>(var))) +
				"]";
		}
		else if (std::holds_alternative<Int64Tuple3>(var))
		{
			return "[" +
				Utils::toString(std::get<0>(std::get<Int64Tuple3>(var))) + ", " +
				Utils::toString(std::get<1>(std::get<Int64Tuple3>(var))) + ", " +
				Utils::toString(std::get<2>(std::get<Int64Tuple3>(var))) +
				"]";
		}
		else if (std::holds_alternative<Int64Tuple4>(var))
		{
			return "[" +
				Utils::toString(std::get<0>(std::get<Int64Tuple4>(var))) + ", " +
				Utils::toString(std::get<1>(std::get<Int64Tuple4>(var))) + ", " +
				Utils::toString(std::get<2>(std::get<Int64Tuple4>(var))) + ", " +
				Utils::toString(std::get<3>(std::get<Int64Tuple4>(var))) +
				"]";
		}
		else if (std::holds_alternative<DoubleTuple2>(var))
		{
			return "[" +
				Utils::toString(std::get<0>(std::get<DoubleTuple2>(var))) + ", " +
				Utils::toString(std::get<1>(std::get<DoubleTuple2>(var))) +
				"]";
		}
		else if (std::holds_alternative<DoubleTuple3>(var))
		{
			return "[" +
				Utils::toString(std::get<0>(std::get<DoubleTuple3>(var))) + ", " +
				Utils::toString(std::get<1>(std::get<DoubleTuple3>(var))) + ", " +
				Utils::toString(std::get<2>(std::get<DoubleTuple3>(var))) +
				"]";
		}
		else if (std::holds_alternative<DoubleTuple4>(var))
		{
			return "[" +
				Utils::toString(std::get<0>(std::get<DoubleTuple4>(var))) + ", " +
				Utils::toString(std::get<1>(std::get<DoubleTuple4>(var))) + ", " +
				Utils::toString(std::get<2>(std::get<DoubleTuple4>(var))) + ", " +
				Utils::toString(std::get<3>(std::get<DoubleTuple4>(var))) +
				"]";
		}
		return {};
	}
}
