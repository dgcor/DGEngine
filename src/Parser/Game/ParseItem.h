#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	std::shared_ptr<Item> parseItemObj(Game& game,
		const Level& level, const rapidjson::Value& elem);
	void parseItem(Game& game, const rapidjson::Value& elem);
}
