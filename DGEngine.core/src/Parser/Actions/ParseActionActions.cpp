#include "ParseActionActions.h"
#include "Game/Actions/ActAction.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;

	std::shared_ptr<Action> parseActionSet(Game& game, const Value& elem)
	{
		return std::make_shared<ActActionSet>(
			getStringViewKey(elem, "id"),
			getActionKey(game, elem, "action"));
	}
}
