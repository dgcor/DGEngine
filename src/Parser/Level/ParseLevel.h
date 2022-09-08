#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseLevel(Game& game, const rapidjson::Value& elem);
}
