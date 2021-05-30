#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Game;

namespace sf
{
	class Image;
	class Texture;
}

namespace Parser
{
	sf::Image parseTextureImg(Game& game, const rapidjson::Value& elem);

	bool parseTextureFromId(Game& game, const rapidjson::Value& elem);

	std::shared_ptr<sf::Texture> getTextureObj(Game& game, const rapidjson::Value& elem);

	typedef decltype(&getTextureObj) getTextureObjFuncPtr;

	void parseTexture(Game& game, const rapidjson::Value& elem,
		const getTextureObjFuncPtr getTextureObjFunc);

	void parseTexture(Game& game, const rapidjson::Value& elem);
}
