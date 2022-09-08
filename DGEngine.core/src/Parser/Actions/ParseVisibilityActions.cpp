#include "ParseVisibilityActions.h"
#include "Game/Actions/ActVisibility.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;

	std::shared_ptr<Action> parseToggleVisible(const Value& elem)
	{
		return std::make_shared<ActToggleVisible>(getStringViewKey(elem, "id"));
	}

	std::shared_ptr<Action> parseSetVisible(const Value& elem)
	{
		return std::make_shared<ActSetVisible>(
			getStringViewKey(elem, "id"),
			getBoolKey(elem, "visible", true));
	}
}
