#include "ParseFontActions.h"
#include "Game/Actions/ActFont.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;

	std::shared_ptr<Action> parseFontSetPaletteOrColor(const Value& elem)
	{
		return std::make_shared<ActFontSetPaletteOrColor>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "palette"),
			getColorKey(elem, "color", sf::Color::White));
	}

	std::shared_ptr<Action> parseFontSetSmooth(const Value& elem)
	{
		return std::make_shared<ActFontSetSmooth>(
			getStringViewKey(elem, "id"),
			getBoolKey(elem, "smooth", true));
	}
}
