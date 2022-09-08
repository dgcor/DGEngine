#pragma once

#include "Json/JsonParser.h"

class Game;
class Level;

namespace Parser
{
	void parseLevelAutomap(Game& game, Level& level, const rapidjson::Value& elem);
}
