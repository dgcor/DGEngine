#include "ParseFadeActions.h"
#include "Game/Actions/ActFade.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Action> parseGameFade(Game& game, const Value& elem, sf::Color defautlColor, bool isFadeOut)
	{
		auto action = std::make_shared<ActGameFade>(
			getColorKey(elem, "color", defautlColor),
			isFadeOut,
			getBoolKey(elem, "enableInput", !isFadeOut),
			(uint8_t)getUIntKey(elem, "fade", 25),
			getTimeUKey(elem, "refresh", sf::milliseconds(15)));

		if (elem.HasMember("action"sv))
		{
			action->setAction(getActionVal(game, elem["action"sv]));
		}
		return action;
	}

	std::shared_ptr<Action> parseGameFadeIn(Game& game, const Value& elem)
	{
		return parseGameFade(game, elem, {}, false);
	}

	std::shared_ptr<Action> parseGameFadeOut(Game& game, const Value& elem)
	{
		return parseGameFade(game, elem, sf::Color::Transparent, true);
	}
}
