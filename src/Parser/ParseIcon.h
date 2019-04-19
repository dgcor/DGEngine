#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseIcon(Game& game, const rapidjson::Value& elem);
}
