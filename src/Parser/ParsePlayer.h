#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parsePlayer(Game& game, const rapidjson::Value& elem);
}
