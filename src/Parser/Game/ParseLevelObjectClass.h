#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseLevelObjectClass(Game& game, const rapidjson::Value& elem);
}
