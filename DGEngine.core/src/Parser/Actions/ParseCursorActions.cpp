#include "ParseCursorActions.h"
#include "Game/Actions/ActCursor.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;

	std::shared_ptr<Action> parseCursorCenterOnDrawable(const Value& elem)
	{
		return std::make_shared<ActCursorCenterOnDrawable>(
			getStringViewKey(elem, "id"));
	}

	std::shared_ptr<Action> parseCursorEnableOutline(const Value& elem)
	{
		return std::make_shared<ActCursorEnableOutline>(
			getBoolKey(elem, "enable", true));
	}

	std::shared_ptr<Action> parseCursorPop(const Value& elem)
	{
		return std::make_shared<ActCursorPop>(getBoolKey(elem, "popAll"));
	}

	std::shared_ptr<Action> parseCursorSetColor(const Value& elem)
	{
		return std::make_shared<ActCursorSetColor>(
			getColorKey(elem, "color", sf::Color::White));
	}

	std::shared_ptr<Action> parseCursorSetOutline(const Value& elem)
	{
		return std::make_shared<ActCursorSetOutline>(
			getColorKey(elem, "outline", sf::Color::Transparent),
			getColorKey(elem, "ignore", sf::Color::Transparent));
	}

	std::shared_ptr<Action> parseCursorSetPalette(const Value& elem)
	{
		return std::make_shared<ActCursorSetPalette>(
			getStringViewKey(elem, "palette"),
			getColorKey(elem, "color", sf::Color::White));
	}

	std::shared_ptr<Action> parseCursorSetPosition(const Value& elem)
	{
		return std::make_shared<ActCursorSetPosition>(
			getVector2iKey<sf::Vector2i>(elem, "position"));
	}

	std::shared_ptr<Action> parseCursorShow(const Value& elem)
	{
		return std::make_shared<ActCursorShow>(getBoolKey(elem, "show"));
	}
}
