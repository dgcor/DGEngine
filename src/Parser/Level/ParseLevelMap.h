#pragma once

#include "Json/JsonParser.h"

class Game;
class Level;

namespace Parser
{
	void parseLevelMap(Game& game, Level& level, const rapidjson::Value* queryDoc, const rapidjson::Value& elem);
}
