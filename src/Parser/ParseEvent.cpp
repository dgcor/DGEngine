#include "ParseEvent.h"
#include "ParseAction.h"
#include "ParseUtils.h"

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

		if (elem.HasMember("id"))
		{
			evt.setId(getStringVal(elem["id"]));
		}

		game.Events().addBack(evt);
	}
}
