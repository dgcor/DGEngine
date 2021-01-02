#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseShape(Game& game, const rapidjson::Value& elem);
}
