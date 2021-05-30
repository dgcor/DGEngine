#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Game;
class Item;
class Level;

namespace Parser
{
	std::shared_ptr<Item> parseItemObj(Game& game,
		Level& level, const rapidjson::Value& elem);
	void parseItem(Game& game, const rapidjson::Value& elem);
}
