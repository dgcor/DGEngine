#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseCelTexturePack(Game& game, const rapidjson::Value& elem);
}
