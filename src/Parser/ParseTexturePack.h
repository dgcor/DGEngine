#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseTexturePack(Game& game, const rapidjson::Value& elem);
}
