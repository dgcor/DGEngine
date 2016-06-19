#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseKeyboard(Game& game, const rapidjson::Value& elem);
}
