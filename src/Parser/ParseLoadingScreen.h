#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseLoadingScreen(Game& game, const rapidjson::Value& elem);
}
