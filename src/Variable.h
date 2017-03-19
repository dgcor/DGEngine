#pragma once

#include <cstdint>
#include <string>
#include "Variant.h"

typedef mapbox::util::variant<std::string, int64_t, double, bool> Variable;

namespace VarUtils
{
	std::string toString(const Variable& var, const std::string& defVal = "");
}
