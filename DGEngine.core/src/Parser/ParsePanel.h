#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Game;
class Panel;

namespace Parser
{
	std::shared_ptr<Panel> getPanelObj(Game& game, const rapidjson::Value& elem);

	typedef decltype(&getPanelObj) getPanelObjFuncPtr;

	void parsePanel(Game& game, const rapidjson::Value& elem,
		const getPanelObjFuncPtr getPanelObjFunc);

	void parsePanel(Game& game, const rapidjson::Value& elem);
}
