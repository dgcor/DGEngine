#pragma once

#include "Game/Variable.h"
#include <string_view>
#include "Utils/Number.h"

class SimpleLevelObject;

class SimpleLevelObjectLevelObject
{
public:
	static bool getNumber(const SimpleLevelObject& levelObj, const std::string_view prop, Number32& value);

	static bool getProperty(const SimpleLevelObject& levelObj, const std::string_view prop, Variable& var);
};
