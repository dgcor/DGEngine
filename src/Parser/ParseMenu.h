#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseMenu(Game& game, const rapidjson::Value& elem);
}
