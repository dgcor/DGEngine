#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseBitmapFont(Game& game, const rapidjson::Value& elem);
}
