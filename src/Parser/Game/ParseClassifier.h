#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseClassifier(Game& game, const rapidjson::Value& elem);
}
