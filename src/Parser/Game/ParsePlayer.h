#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parsePlayer(Game& game, const rapidjson::Value& elem);
}
