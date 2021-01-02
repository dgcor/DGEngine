#pragma once

#include <cstdint>
#include "Json/JsonParser.h"
#include "ParserProperties.h"

class Game;

namespace Parser
{
	void parseDocumentGameElem(Game& game, uint16_t nameHash16, const rapidjson::Value& elem,
		ReplaceVars& replaceVars, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator);
}
