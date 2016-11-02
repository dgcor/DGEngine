#include "ParseFont.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	bool parseFontFromId(Game& game, const Value& elem)
	{
		if (isValidString(elem, "fromId") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				std::string fromId(elem["fromId"].GetString());
				std::string id(elem["id"].GetString());
				if (fromId != id && isValidId(id) == true)
				{
					auto obj = game.Resources().getFont(fromId);
					if (obj != nullptr)
					{
						game.Resources().addFont(id, obj);
					}
				}
			}
			return true;
		}
		return false;
	}

	void parseFont(Game& game, const Value& elem)
	{
		if (parseFontFromId(game, elem) == true)
		{
			return;
		}

		if (isValidString(elem, "file") == false)
		{
			return;
		}

		std::string file(elem["file"].GetString());
		std::string id;

		if (isValidString(elem, "id") == true)
		{
			id = elem["id"].GetString();
		}
		else if (getIdFromFile(file, id) == false)
		{
			return;
		}
		if (isValidId(id) == false)
		{
			return;
		}

		auto font = std::make_shared<Font2>(std::make_shared<sf::PhysFSStream>(file));
		if (font->load() == false)
		{
			return;
		}

		game.Resources().addFont(id, font);
	}
}
