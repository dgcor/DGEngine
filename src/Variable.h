#pragma once

#include <cstdint>
#include <string>
#include "variant/variant.hpp"

typedef mapbox::util::variant<std::string, int64_t, double, bool> Variable;

namespace VarUtils
{
	std::string toString(const Variable& var, const std::string& defVal = "");
}
