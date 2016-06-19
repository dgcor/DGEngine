#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseMountFile(Game& game, const rapidjson::Value& elem);
}
