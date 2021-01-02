#include "ParseCursor.h"
#include "Animation.h"
#include <cmath>
#include "Game.h"
#include "ParseAnimation.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	sf::Vector2f getOrigin(const Value& elem, float width, float height)
	{
		if (elem.HasMember("origin"sv) &&
			elem["origin"sv].IsString() &&
			elem["origin"sv].GetStringView() == "center")
		{
			return sf::Vector2f(std::round(width / 2.f), std::round(height / 2.f));
		}
		else
		{
			return getVector2fKey<sf::Vector2f>(elem, "origin");
		}
	}

	void parseCursor(Game& game, const Value& elem)
	{
		if (elem.IsNull() == true)
		{
			game.Resources().addCursor(nullptr);
			return;
		}
		else if (elem.IsObject() == false)
		{
			return;
		}

		if (elem.HasMember("show"sv) == true)
		{
			game.setMouseCursorVisible(getBoolVal(elem["show"sv]));
		}

		if (getBoolKey(elem, "pop") == true)
		{
			game.Resources().popCursor();
			game.updateMousePosition();
		}

		auto cursor = getAnimationObj(game, elem);
		if (cursor == nullptr)
		{
			return;
		}
		auto size = cursor->Size();
		cursor->setOrigin(getOrigin(elem, size.x, size.y));
		game.Resources().addCursor(cursor);
		game.updateMousePosition();
	}
}
