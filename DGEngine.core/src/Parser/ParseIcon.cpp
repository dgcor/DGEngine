#include "ParseIcon.h"
#include "Game.h"
#include "ImageUtils.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseIcon(Game& game, const Value& elem)
	{
		if (isValidString(elem, "file") == false)
		{
			return;
		}

		auto icon = ImageUtils::loadImage(getStringViewVal(elem["file"sv]));
		auto iconSize = icon.getSize();
		if (iconSize.x == 0 || iconSize.y == 0)
		{
			return;
		}
		game.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}
}
