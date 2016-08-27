#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseEvent(Game& game, const rapidjson::Value& elem);
}
