#include "ParseButtonActions.h"
#include "Game/Actions/ActButton.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;

	std::shared_ptr<Action> parseButtonClick(const Value& elem)
	{
		return std::make_shared<ActButtonClick>(
			getStringViewKey(elem, "id"),
			getBoolKey(elem, "playSound", true));
	}

	std::shared_ptr<Action> parseButtonEnable(const Value& elem)
	{
		return std::make_shared<ActButtonEnable>(
			getStringViewKey(elem, "id"),
			getBoolKey(elem, "enable", true));
	}

	std::shared_ptr<Action> parseButtonRightClick(const Value& elem)
	{
		return std::make_shared<ActButtonRightClick>(
			getStringViewKey(elem, "id"),
			getBoolKey(elem, "playSound", true));
	}

	std::shared_ptr<Action> parseButtonSetColor(const Value& elem)
	{
		return std::make_shared<ActButtonSetColor>(
			getStringViewKey(elem, "id"),
			getColorKey(elem, "color", sf::Color::White));
	}
}
