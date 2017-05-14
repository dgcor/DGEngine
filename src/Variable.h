#pragma once

#include <cstdint>
#include <string>
#include "Variant.h"

typedef mapbox::util::variant<std::string, int64_t, double, bool> Variable;

namespace VarUtils
{
	bool toBool(const Variable& var);
	double toDouble(const Variable& var);
	int64_t toLong(const Variable& var);
	std::string toString(const Variable& var);
}
