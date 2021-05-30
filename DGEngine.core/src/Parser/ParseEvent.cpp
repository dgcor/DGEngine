#include "ParseEvent.h"
#include "Game.h"
#include "ParseAction.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

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
			auto id = getStringViewVal(elem["id"sv]);
			if (isValidId(id) == true)
			{
				evt->setId(id);
			}
		}
		if (getBoolKey(elem, "addToFront") == true)
		{
			game.Events().addFront(evt);
		}
		else
		{
			game.Events().addBack(evt);
		}
	}
}
