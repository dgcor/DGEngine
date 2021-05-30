#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Game;
class Image;

namespace Parser
{
	std::shared_ptr<Image> getImageObj(Game& game, const rapidjson::Value& elem);

	typedef decltype(&getImageObj) getImageObjFuncPtr;

	void parseImage(Game& game, const rapidjson::Value& elem,
		const getImageObjFuncPtr getImageObjFunc);

	void parseImage(Game& game, const rapidjson::Value& elem);
}
