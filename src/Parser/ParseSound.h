#pragma once

#include "Json/JsonParser.h"
#include <SFML/Audio/SoundBuffer.hpp>
#include <string>
#include <string_view>

class Game;

namespace Parser
{
	sf::SoundBuffer* parseSoundObj(Game& game, const std::string& id,
		const std::string& file, const std::string_view resource);
	void parseSound(Game& game, const rapidjson::Value& elem);
}
