#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseFont(Game& game, const rapidjson::Value& elem);
}
