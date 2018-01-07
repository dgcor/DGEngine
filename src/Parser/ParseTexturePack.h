#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseTexturePack(Game& game, const rapidjson::Value& elem);

	void getOrParseLevelTexturePack(Game& game, const rapidjson::Value& elem,
		const char* idKeyBottom, const char* idKeyTop,
		std::shared_ptr<TexturePack>& texturePackBottom,
		std::shared_ptr<TexturePack>& texturePackTop);
}
