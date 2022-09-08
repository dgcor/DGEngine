#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	bool cloneFont(Game& game, const rapidjson::Value& elem);

	bool parseBitmapFont(Game& game, const rapidjson::Value& elem);

	bool parseFreeTypeFont(Game& game, const rapidjson::Value& elem);

	void parseFont(Game& game, const rapidjson::Value& elem);
}
