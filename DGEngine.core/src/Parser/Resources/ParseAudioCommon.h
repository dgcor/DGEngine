#pragma once

#include "Json/JsonParser.h"
#include "SFML/MusicLoops.h"
#include <string>
#include <string_view>
#include "Utils/UnorderedStringMap.h"
#include <vector>

namespace Parser
{
	sf::Music::TimeSpan parseAudioTimeSpan(const rapidjson::Value& elem);

	void parseAudioLoopName(const rapidjson::Value& elem,
		UnorderedStringMap<sf::Music::TimeSpan>& loopNames);

	void parseAudioLoopNamesVal(const rapidjson::Value& elem, const char* key,
		UnorderedStringMap<sf::Music::TimeSpan>& loopNames);

	void parseAudioLoopPoint(const rapidjson::Value& elem,
		std::vector<sf::Music::TimeSpan>& loopPoints);

	void parseAudioLoopPointsVal(const rapidjson::Value& elem,
		const char* key, sf::MusicLoops& music);

	void updateAudioLoopString(const std::string_view str,
		const UnorderedStringMap<sf::Music::TimeSpan>& loopNames,
		sf::MusicLoops& music);
}
