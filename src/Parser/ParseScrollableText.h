#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseScrollableText(Game& game, const rapidjson::Value& elem);
}
