#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseLevel(Game& game, const rapidjson::Value& elem);
}
