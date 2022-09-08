#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	void parseGameElem(Game& game, uint16_t nameHash16, const rapidjson::Value& elem);

	void parseGame(Game& game, const rapidjson::Value& elem);
}
