#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Game;
class ImageContainer;

namespace Parser
{
	std::shared_ptr<ImageContainer> getImageContainerObj(Game& game,
		const rapidjson::Value& elem);

	bool parseImageContainerFromId(Game& game, const rapidjson::Value& elem);

	typedef decltype(&getImageContainerObj) getImageContainerObjFuncPtr;

	void parseImageContainer(Game& game, const rapidjson::Value& elem,
		const getImageContainerObjFuncPtr getImageContainerObjFunc);

	void parseImageContainer(Game& game, const rapidjson::Value& elem);
}
