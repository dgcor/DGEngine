#pragma once
#if (SFML_VERSION_MAJOR > 2 || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR >= 5))
#include "Json/JsonParser.h"
#include "MusicLoops.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace Parser
{
	sf::Music::TimeSpan parseAudioTimeSpan(const rapidjson::Value& elem);

	void parseAudioLoopName(const rapidjson::Value& elem,
		std::unordered_map<std::string, sf::Music::TimeSpan>& loopNames);

	void parseAudioLoopNamesVal(const rapidjson::Value& elem, const char* key,
		std::unordered_map<std::string, sf::Music::TimeSpan>& loopNames);

	void parseAudioLoopPoint(const rapidjson::Value& elem,
		std::vector<sf::Music::TimeSpan>& loopPoints);

	void parseAudioLoopPointsVal(const rapidjson::Value& elem,
		const char* key, sf::MusicLoops& music);

	void updateAudioLoopString(const std::string& str,
		const std::unordered_map<std::string, sf::Music::TimeSpan>& loopNames,
		sf::MusicLoops& music);
}
#endif
