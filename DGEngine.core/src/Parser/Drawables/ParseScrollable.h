#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Game;
class Scrollable;

namespace Parser
{
	std::shared_ptr<Scrollable> getScrollableObj(Game& game, const rapidjson::Value& elem);

	void parseScrollable(Game& game, const rapidjson::Value& elem);
}
