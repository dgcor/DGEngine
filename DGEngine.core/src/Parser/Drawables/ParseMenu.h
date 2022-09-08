#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Game;
class Menu;

namespace Parser
{
	std::shared_ptr<Menu> getMenuObj(Game& game, const rapidjson::Value& elem);

	void parseMenu(Game& game, const rapidjson::Value& elem);
}
