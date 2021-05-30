#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser2
{
	void parseTexturePack(Game& game, const rapidjson::Value& elem);
}
