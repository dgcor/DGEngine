#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseSpellClass(Game& game, const rapidjson::Value& elem);
}
