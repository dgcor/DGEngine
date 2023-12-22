#include "ParseAnimationActions.h"
#include "Game/Actions/ActAnimation.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;

	std::shared_ptr<Action> parseAnimationPause(const Value& elem)
	{
		return std::make_shared<ActAnimationPause>(
			getStringViewKey(elem, "id"),
			getBoolKey(elem, "pause", true),
			getBoolKey(elem, "reset"));
	}

	std::shared_ptr<Action> parseAnimationSetAnimation(const Value& elem)
	{
		return std::make_shared<ActAnimationSetAnimation>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "texturePack"),
			getIntKey(elem, "group", -1),
			getIntKey(elem, "direction", -1),
			getTimeUKey(elem, "refresh"),
			getBoolKey(elem, "reset", true),
			getBoolKey(elem, "updateAnimationType"));
	}

	std::shared_ptr<Action> parseAnimationSetRefresh(const Value& elem)
	{
		return std::make_shared<ActAnimationSetRefresh>(
			getStringViewKey(elem, "id"),
			getTimeUKey(elem, "refresh"));
	}
}
