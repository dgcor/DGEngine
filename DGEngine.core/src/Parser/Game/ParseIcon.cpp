#include "ParseIcon.h"
#include "Game/Game.h"
#include "Game/Utils/ImageUtils.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseIcon(Game& game, const Value& elem)
	{
		if (isValidString(elem) == false)
		{
			return;
		}
		auto icon = ImageUtils::loadImage(elem.GetStringView());
		auto iconSize = icon.getSize();
		if (iconSize.x == 0 || iconSize.y == 0)
		{
			return;
		}
		game.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}
}
