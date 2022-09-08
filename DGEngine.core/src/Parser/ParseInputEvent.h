#pragma once

#include "Json/JsonParser.h"
#include <vector>

class Game;

namespace sf
{
	class Event;
}

namespace Parser
{
	sf::Event updateKeyEvent(sf::Event evt);

	std::vector<sf::Event> parseInputEvents(const rapidjson::Value& elem);

	void parseActionKey(Game& game, const rapidjson::Value& elem);

	void parseEventKey(Game& game, const rapidjson::Value& elem);

	void parseInputEvent(Game& game, const rapidjson::Value& elem);
}
