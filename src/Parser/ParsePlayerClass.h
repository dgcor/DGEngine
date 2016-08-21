#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parsePlayerClass(Game& game, const rapidjson::Value& elem);
}
