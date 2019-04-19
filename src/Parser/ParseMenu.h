#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseMenu(Game& game, const rapidjson::Value& elem);
}
