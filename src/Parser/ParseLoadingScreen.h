#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseLoadingScreen(Game& game, const rapidjson::Value& elem);
}
