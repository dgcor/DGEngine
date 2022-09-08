#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Game;
class InputText;

namespace Parser
{
	std::shared_ptr<InputText> getInputTextObj(Game& game, const rapidjson::Value& elem);

	void parseInputText(Game& game, const rapidjson::Value& elem);
}
