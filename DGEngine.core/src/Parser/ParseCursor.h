#pragma once

#include "ParseAnimation.h"

namespace Parser
{
	void parseCursor(Game& game, const rapidjson::Value& elem,
		const getAnimationObjFuncPtr getAnimationObjFunc);

	void parseCursor(Game& game, const rapidjson::Value& elem);
}
