#include "ParseLoadingScreenActions.h"
#include "Game/Actions/ActLoadingScreen.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;

	std::shared_ptr<Action> parseLoadingScreenClear()
	{
		return std::make_shared<ActLoadingScreenClear>();
	}

	std::shared_ptr<Action> parseLoadingScreenSetProgress(const Value& elem)
	{
		return std::make_shared<ActLoadingScreenSetProgress>(getIntKey(elem, "progress"));
	}
}
