#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseCompositeTexture(Game& game, const rapidjson::Value& elem);
}
