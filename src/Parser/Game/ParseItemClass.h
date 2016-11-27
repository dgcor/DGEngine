#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseItemClass(Game& game, const rapidjson::Value& elem);
}
