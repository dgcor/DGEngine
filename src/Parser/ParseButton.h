#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseButton(Game& game, const rapidjson::Value& elem);
}
