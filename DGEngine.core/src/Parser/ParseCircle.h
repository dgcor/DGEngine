#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Circle;
class Game;

namespace Parser
{
	std::shared_ptr<Circle> getCircleObj(Game& game, const rapidjson::Value& elem);

	typedef decltype(&getCircleObj) getCircleObjFuncPtr;

	void parseCircle(Game& game, const rapidjson::Value& elem,
		const getCircleObjFuncPtr getCircleObjFunc);

	void parseCircle(Game& game, const rapidjson::Value& elem);
}
