#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseMonsterClass(Game& game, const rapidjson::Value& elem);
}
