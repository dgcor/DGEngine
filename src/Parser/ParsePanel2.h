#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser2
{
	void parsePanel(Game& game, const rapidjson::Value& elem);
}
