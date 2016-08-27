#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseCircle(Game& game, const rapidjson::Value& elem);
}
