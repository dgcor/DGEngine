#pragma once

#include "Json/JsonParser.h"
#include <SFML/Graphics/Image.hpp>

class Game;

namespace Parser2
{
	bool parseTextureImg(Game& game, const rapidjson::Value& elem, sf::Image& img);
}
