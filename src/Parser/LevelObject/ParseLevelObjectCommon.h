#pragma once

#include "Json/JsonParser.h"

class Level;
class LevelObjectQueryable;

namespace Parser
{
	bool parseLevelObjectProperties(LevelObjectQueryable& levelObj, Level& level, const rapidjson::Value& elem);
}
