#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	std::shared_ptr<BitmapFontTexturePack> parseBitmapFontTexturePackObj(
		Game& game, const rapidjson::Value& elem);

	std::shared_ptr<TexturePack> parseTexturePackObj(Game& game, const rapidjson::Value& elem);

	void parseTexturePack(Game& game, const rapidjson::Value& elem);

	bool getOrParseLevelTexturePack(Game& game, const rapidjson::Value& elem,
		const char* idKeyLayers, std::vector<std::shared_ptr<TexturePack>>& texturePackLayers,
		std::pair<int32_t, int32_t>& tileSize);
}
