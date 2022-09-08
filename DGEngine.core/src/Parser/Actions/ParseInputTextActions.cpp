#include "ParseInputTextActions.h"
#include "Game/Actions/ActInputText.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;

	std::shared_ptr<Action> parseInputTextClick(const Value& elem)
	{
		return std::make_shared<ActInputTextClick>(getStringViewKey(elem, "id"));
	}
}
