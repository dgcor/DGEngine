#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseCircle(Game& game, const rapidjson::Value& elem);
}
