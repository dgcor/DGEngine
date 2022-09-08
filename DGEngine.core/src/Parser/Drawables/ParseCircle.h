#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Circle;
class Game;

namespace Parser
{
	std::shared_ptr<Circle> getCircleObj(Game& game, const rapidjson::Value& elem);

	void parseCircle(Game& game, const rapidjson::Value& elem);
}
