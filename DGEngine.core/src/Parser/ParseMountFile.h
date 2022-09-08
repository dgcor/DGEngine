#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseMountFile(Game& game, const rapidjson::Value& elem);
}
