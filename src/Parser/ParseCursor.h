#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseCursor(Game& game, const rapidjson::Value& elem);
}
