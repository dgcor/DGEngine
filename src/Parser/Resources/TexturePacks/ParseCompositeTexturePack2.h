#pragma once

#include "Json/JsonParser.h"
#include <memory>

class CompositeTexturePack2;
class Game;

namespace Parser2
{
	std::shared_ptr<CompositeTexturePack2> getCompositeTexturePackObj(Game& game, const rapidjson::Value& elem);
}
