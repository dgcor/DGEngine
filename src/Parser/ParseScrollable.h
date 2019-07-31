#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseScrollable(Game& game, const rapidjson::Value& elem);
}
