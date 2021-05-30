#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Animation;
class Game;

namespace Parser
{
	std::shared_ptr<Animation> getAnimationObj(Game& game, const rapidjson::Value& elem);

	typedef decltype(&getAnimationObj) getAnimationObjFuncPtr;

	void parseAnimation(Game& game, const rapidjson::Value& elem,
		const getAnimationObjFuncPtr getAnimationObjFunc);

	void parseAnimation(Game& game, const rapidjson::Value& elem);
}
