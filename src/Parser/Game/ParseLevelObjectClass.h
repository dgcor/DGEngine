#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseLevelObjectClass(Game& game, const rapidjson::Value& elem);
}
