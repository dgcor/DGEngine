#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	sf::Image parseTextureImg(Game& game, const rapidjson::Value& elem,
		size_t* numFramesX = nullptr, size_t* numFramesY = nullptr);
	void parseTexture(Game& game, const rapidjson::Value& elem);
}
