#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseFile(Game& game, const std::string& fileName);
	void parseFile(Game& game, const std::vector<std::string>& params);
	void parseFile(Game& game, const rapidjson::Value& params);
	void parseDocument(Game& game, const rapidjson::Document& doc);
	void parseJson(Game& game, const std::string& json);
}
