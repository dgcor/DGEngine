#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseMountFile(const Game& game, const rapidjson::Value& elem);
}
