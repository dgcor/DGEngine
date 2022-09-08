#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parsePalette(Game& game, const rapidjson::Value& elem);
}
