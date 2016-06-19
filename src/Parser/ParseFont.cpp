#include "ParseFont.h"
#include "ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseFont(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false || isValidString(elem, "file") == false)
		{
			return;
		}

		auto font = std::make_shared<Font2>(std::make_shared<sf::PhysFSStream>(elem["file"].GetString()));
		if (font->load() == false)
		{
			return;
		}

		game.Resources().addFont(elem["id"].GetString(), font);
	}
}
