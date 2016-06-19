#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseRectangle(Game& game, const rapidjson::Value& elem);
}
