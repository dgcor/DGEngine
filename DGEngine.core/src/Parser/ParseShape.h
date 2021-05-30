#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Game;
class Shape;

namespace Parser
{
	std::shared_ptr<Shape> getShapeObj(Game& game, const rapidjson::Value& elem);

	typedef decltype(&getShapeObj) getShapeObjFuncPtr;

	void parseShape(Game& game, const rapidjson::Value& elem,
		const getShapeObjFuncPtr getShapeObjFunc);

	void parseShape(Game& game, const rapidjson::Value& elem);
}
