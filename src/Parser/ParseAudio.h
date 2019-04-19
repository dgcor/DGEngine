#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseAudio(Game& game, const rapidjson::Value& elem);
}
