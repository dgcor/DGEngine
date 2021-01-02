#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseInputEvent(Game& game, const rapidjson::Value& elem);
}
