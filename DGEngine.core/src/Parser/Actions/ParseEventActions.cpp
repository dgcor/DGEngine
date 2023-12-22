#include "ParseEventActions.h"
#include "Game/Actions/ActEvent.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;

	std::shared_ptr<Action> parseEventAdd(Game& game, const Value& elem)
	{
		return std::make_shared<ActEventAdd>(
			getStringViewKey(elem, "id"),
			getActionKey(game, elem, "action"),
			getTimeUKey(elem, "time"),
			getBoolKey(elem, "addToFront"));
	}

	std::shared_ptr<Action> parseEventDelete(const Value& elem)
	{
		return std::make_shared<ActEventDelete>(getStringViewKey(elem, "id"));
	}

	std::shared_ptr<Action> parseEventDeleteAll()
	{
		return std::make_shared<ActEventDeleteAll>();
	}

	std::shared_ptr<Action> parseEventResetTime(const Value& elem)
	{
		return std::make_shared<ActEventResetTime>(getStringViewKey(elem, "id"));
	}

	std::shared_ptr<Action> parseEventTimeout(const Value& elem)
	{
		return std::make_shared<ActEventTimeout>(getStringViewKey(elem, "id"));
	}
}
