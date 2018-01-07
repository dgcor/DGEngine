#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	sf::Image parseTextureImg(Game& game, const rapidjson::Value& elem);
	std::shared_ptr<sf::Texture> parseTextureObj(Game& game,
		const rapidjson::Value& elem, const sf::Image& image);
	std::shared_ptr<sf::Texture> parseTextureObj(Game& game,
		const rapidjson::Value& elem);
	void parseTexture(Game& game, const rapidjson::Value& elem);

	bool getOrParseTexture(Game& game, const rapidjson::Value& elem,
		const char* idKey, std::shared_ptr<sf::Texture>& texture);
}
