#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseImage(Game& game, const rapidjson::Value& elem);
}
