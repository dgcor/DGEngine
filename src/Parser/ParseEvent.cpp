#include "ParseEvent.h"
#include "ParseAction.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseEvent(Game& game, const Value& elem)
	{
		auto action = parseAction(game, elem["action"]);

		float seconds = 0.01f;
		if (elem.HasMember("seconds"))
		{
			const auto& elemSec = elem["seconds"];

			if (elemSec.IsUint())
			{
				seconds = (float)elemSec.GetUint();
			}
			else if (elemSec.IsDouble())
			{
				seconds = (float)elemSec.GetDouble();
			}
		}

		auto evt = Event(action, sf::seconds(seconds));

		if (isValidString(elem, "id") == true)
		{
			std::string id(elem["id"].GetString());
			if (isValidId(id) == true)
			{
				evt.setId(id);
			}
		}

		game.Events().addBack(evt);
	}
}
