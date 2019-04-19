#pragma once

#include "Json/JsonParser.h"
#include <memory>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

class Game;

namespace Parser
{
	struct ImageCache
	{
		sf::Image image;
		std::string path;
	};

	sf::Image parseTextureImg(Game& game, const rapidjson::Value& elem);

	// returns true if image is the same as the cache and leaves img untouched
	bool parseTextureImg(Game& game, const rapidjson::Value& elem,
		sf::Image& img, ImageCache* cache);

	std::shared_ptr<sf::Texture> parseTextureObj(Game& game,
		const rapidjson::Value& elem, const sf::Image& image);

	std::shared_ptr<sf::Texture> parseTextureObj(Game& game,
		const rapidjson::Value& elem);
	std::shared_ptr<sf::Texture> parseTextureObj(Game& game,
		const rapidjson::Value& elem, ImageCache* cache);

	void parseTexture(Game& game, const rapidjson::Value& elem);
	void parseTexture(Game& game, const rapidjson::Value& elem, ImageCache* cache);

	bool getOrParseTexture(Game& game, const rapidjson::Value& elem,
		const char* idKey, std::shared_ptr<sf::Texture>& texture);
	bool getOrParseTexture(Game& game, const rapidjson::Value& elem,
		const char* idKey, std::shared_ptr<sf::Texture>& texture, ImageCache* cache);
}
