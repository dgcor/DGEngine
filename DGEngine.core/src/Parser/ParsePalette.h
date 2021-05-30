#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	bool parsePaletteFromId(Game& game, const rapidjson::Value& elem);

	void parsePalette(Game& game, const rapidjson::Value& elem);
}
