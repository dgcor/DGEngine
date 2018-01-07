#include "ParseEvent.h"
#include "ParseAction.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseEvent(Game& game, const Value& elem)
	{
		auto action = getActionKey(game, elem, "action");
		if (action == nullptr)
		{
			return;
		}

		auto timeout = getTimeKey(elem, "time", sf::milliseconds(10));
		auto evt = std::make_shared<Event>(action, timeout);

		if (isValidString(elem, "id") == true)
		{
			std::string id(elem["id"].GetString());
			if (isValidId(id) == true)
			{
				evt->setId(id);
			}
		}

		game.Events().addBack(evt);
	}
}
