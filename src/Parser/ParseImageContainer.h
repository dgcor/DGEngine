#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Game;
class ImageContainer;

namespace Parser
{
	std::shared_ptr<ImageContainer> getImageContainerObj(Game& game,
		const rapidjson::Value& elem);

	void parseImageContainer(Game& game, const rapidjson::Value& elem);
}
