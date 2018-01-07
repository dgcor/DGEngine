#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	std::shared_ptr<ImageContainer> parseImageContainerObj(Game& game,
		const rapidjson::Value& elem);
	void parseImageContainer(Game& game, const rapidjson::Value& elem);

	bool getOrParseImageContainer(Game& game, const rapidjson::Value& elem,
		const char* idKey, std::shared_ptr<ImageContainer>& imgContainer);
}
