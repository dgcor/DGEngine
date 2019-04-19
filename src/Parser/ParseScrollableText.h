#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseScrollableText(Game& game, const rapidjson::Value& elem);
}
