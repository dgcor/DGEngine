#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Game;
class TexturePack;

namespace Parser2
{
	std::unique_ptr<TexturePack> parseImageContainerTexturePack(Game& game, const rapidjson::Value& elem);
}
