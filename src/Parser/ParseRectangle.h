#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseRectangle(Game& game, const rapidjson::Value& elem);
}
