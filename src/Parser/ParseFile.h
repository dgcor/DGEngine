#pragma once

#include "Json/JsonParser.h"
#include "ParserProperties.h"
#include <string>
#include <string_view>
#include <vector>

class Game;

namespace Parser
{
	void parseFile(Game& game, const std::string_view fileName);

	void parseFile(Game& game, const std::vector<std::string>& params);

	void parseDocument(Game& game, const rapidjson::Document& doc,
		ReplaceVars replaceVars_ = ReplaceVars::None);

	void parseDocumentElemHelper(Game& game, uint16_t nameHash16, const rapidjson::Value& elem,
		ReplaceVars& replaceVars, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator);

	void parseJson(Game& game, const std::string_view json,
		const std::vector<std::string>& params);

	void parseJson(Game& game, const std::string_view json);
}
