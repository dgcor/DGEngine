#include "ParseGameInputEvent.h"
#include "Game/Game.h"
#include "ParseInputEvent.h"
#include "ParseAction.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseGameInputEvent(Game& game, const Value& elem)
	{
		auto key = getStringKey(elem, "name");

		auto inputEvents = parseInputEvents(elem);
		if (inputEvents.empty() == true)
		{
			return;
		}

		for (const auto& evt : inputEvents)
		{
			game.GameInputEvents().set(key, evt);
		}
	}
}
