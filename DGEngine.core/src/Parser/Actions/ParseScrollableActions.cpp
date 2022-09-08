#include "ParseScrollableActions.h"
#include "Game/Actions/ActScrollable.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;

	std::shared_ptr<Action> parseScrollableSetSpeed(const Value& elem)
	{
		return std::make_shared<ActScrollableSetSpeed>(
			getStringViewKey(elem, "id"),
			getTimeKey(elem, "speed"));
	}
}
