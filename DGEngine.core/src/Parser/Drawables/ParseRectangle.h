#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Game;
class Rectangle;

namespace Parser
{
	std::shared_ptr<Rectangle> getRectangleObj(Game& game, const rapidjson::Value& elem);

	void parseRectangle(Game& game, const rapidjson::Value& elem);
}
