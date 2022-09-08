#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parsePlayerClass(Game& game, const rapidjson::Value& elem);
}
