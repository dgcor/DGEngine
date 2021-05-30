#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Game;
class Rectangle;

namespace Parser
{
	std::shared_ptr<Rectangle> getRectangleObj(Game& game, const rapidjson::Value& elem);

	typedef decltype(&getRectangleObj) getRectangleObjFuncPtr;

	void parseRectangle(Game& game, const rapidjson::Value& elem,
		const getRectangleObjFuncPtr getRectangleObjFunc);

	void parseRectangle(Game& game, const rapidjson::Value& elem);
}
