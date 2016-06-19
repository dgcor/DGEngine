#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseFont(Game& game, const rapidjson::Value& elem);
}
