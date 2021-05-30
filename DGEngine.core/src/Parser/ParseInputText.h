#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Game;
class InputText;

namespace Parser
{
	std::shared_ptr<InputText> getInputTextObj(Game& game, const rapidjson::Value& elem);

	typedef decltype(&getInputTextObj) getInputTextObjFuncPtr;

	void parseInputText(Game& game, const rapidjson::Value& elem,
		const getInputTextObjFuncPtr getInputTextObjFunc);

	void parseInputText(Game& game, const rapidjson::Value& elem);
}
