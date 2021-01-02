#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseFileBytes(Game& game, const rapidjson::Value& elem);
}
