#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseKeyboard(Game& game, const rapidjson::Value& elem);
}
