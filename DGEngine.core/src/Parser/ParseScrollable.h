#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Game;
class Scrollable;
class UIObject;

namespace Parser
{
	std::shared_ptr<Scrollable> getScrollableObj(Game& game, const rapidjson::Value& elem,
		std::shared_ptr<UIObject>& uiObj);

	typedef decltype(&getScrollableObj) getScrollableObjFuncPtr;

	void parseScrollable(Game& game, const rapidjson::Value& elem,
		const getScrollableObjFuncPtr getScrollableObjFunc);

	void parseScrollable(Game& game, const rapidjson::Value& elem);
}
