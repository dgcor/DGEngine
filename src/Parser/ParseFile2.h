#pragma once

#include <cstdint>
#include "Json/JsonParser.h"
#include "Parser/ParserProperties.h"

class Game;

namespace Parser2
{
	bool parseDocumentElem(Game& game, uint16_t nameHash16, const rapidjson::Value& elem,
		ReplaceVars& replaceVars, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator);
}
