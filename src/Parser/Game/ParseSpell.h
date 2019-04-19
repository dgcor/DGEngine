#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseSpell(Game& game, const rapidjson::Value& elem);
}
