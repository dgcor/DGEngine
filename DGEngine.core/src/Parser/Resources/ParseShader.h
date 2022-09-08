#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseShader(Game& game, const rapidjson::Value& elem);
}
