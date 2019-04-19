#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Game;
class ImageContainer;

namespace Parser
{
	std::shared_ptr<ImageContainer> parseImageContainerObj(Game& game,
		const rapidjson::Value& elem, const char* fileElem = "file");

	void parseImageContainer(Game& game, const rapidjson::Value& elem);

	bool getOrParseImageContainer(Game& game, const rapidjson::Value& elem,
		const char* idKey, std::shared_ptr<ImageContainer>& imgContainer,
		const char* fileElem = "file");
}
