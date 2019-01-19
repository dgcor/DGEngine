#pragma once

#include "Game.h"
#include "Json/JsonParser.h"
#include "ParserProperties.h"

namespace Parser
{
	void parseFile(Game& game, const std::string_view fileName);

	void parseFile(Game& game, const std::vector<std::string>& params);

	void parseDocument(Game& game, const rapidjson::Document& doc,
		ReplaceVars replaceVars_ = ReplaceVars::None);

	void parseJson(Game& game, const std::string_view json,
		const std::vector<std::string>& params);

	void parseJson(Game& game, const std::string_view json);
}
