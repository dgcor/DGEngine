#pragma once

#include "Json/JsonParser.h"
#include <memory>

class BitmapFontTexturePack;
class Game;
class TexturePack;

namespace Parser
{
	std::shared_ptr<BitmapFontTexturePack> parseBitmapFontTexturePackObj(
		Game& game, const rapidjson::Value& elem);

	std::shared_ptr<TexturePack> parseTexturePackObj(Game& game, const rapidjson::Value& elem);

	void parseTexturePack(Game& game, const rapidjson::Value& elem);
}
