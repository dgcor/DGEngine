#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Game;
class Shape;

namespace Parser
{
	std::shared_ptr<Shape> getShapeObj(Game& game, const rapidjson::Value& elem);

	void parseShape(Game& game, const rapidjson::Value& elem);
}
