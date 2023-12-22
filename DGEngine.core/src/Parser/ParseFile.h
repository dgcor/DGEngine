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

	bool parseJson(Game& game, const std::string_view json,
		const std::vector<std::string>& params);

	bool parseJson(Game& game, const std::string_view json);

	void parseDocument(Game& game, rapidjson::Document& doc,
		ReplaceVars replaceVars_ = ReplaceVars::None);

	void parseLoad(Game& game, const rapidjson::Value& elem);

	typedef void(*parseDocumentElemFuncPtr)(Game& game, const rapidjson::Value& elem);

	void parseDocumentElemArray(const parseDocumentElemFuncPtr parseFunc, Game& game,
		uint16_t nameHash16, const rapidjson::Value& elem, ReplaceVars& replaceVars,
		rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator);

	void parseDocumentElemHelper(Game& game, uint16_t nameHash16, const rapidjson::Value& elem,
		ReplaceVars& replaceVars, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator);
}
