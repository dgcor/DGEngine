#pragma once

#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	bool parseFileBytesFromId(Game& game, const rapidjson::Value& elem);

	void parseFileBytes(Game& game, const rapidjson::Value& elem);
}
