#pragma once

#include "Parser/Drawables/ParseAnimation.h"

namespace Parser
{
	void parseCursorF(Game& game, const rapidjson::Value& elem,
		const getAnimationObjFuncPtr getAnimationObjFunc);

	void parseCursor(Game& game, const rapidjson::Value& elem);
}
