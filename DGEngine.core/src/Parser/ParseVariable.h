#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseVariable(Game& game, const rapidjson::Value& elem);
}
