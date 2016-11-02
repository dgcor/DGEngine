#pragma once

#include "Json/JsonParser.h"
#include <SFML/Graphics.hpp>

class Game;

namespace Parser
{
	sf::Color getColorVar(Game& game, const rapidjson::Value& elem, const char* key,
		const sf::Color& val = sf::Color());
}
