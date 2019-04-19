#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseInputText(Game& game, const rapidjson::Value& elem);
}
