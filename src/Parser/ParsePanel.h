#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parsePanel(Game& game, const rapidjson::Value& elem);
}
