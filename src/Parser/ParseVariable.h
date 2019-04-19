#pragma once

#include "Json/JsonParser.h"
#include <string>
#include "Variable.h"
#include <vector>

class Game;

namespace Parser
{
	std::vector<std::pair<std::string, Variable>> getVariables(const rapidjson::Value& elem);

	void parseVariable(Game& game, const rapidjson::Value& elem);
}
