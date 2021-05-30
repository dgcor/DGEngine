#include "ParseAudioCommon.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	sf::Music::TimeSpan parseAudioTimeSpan(const Value& elem)
	{
		return sf::Music::TimeSpan(
			getTimeKey(elem, "offset"),
			getTimeKey(elem, "length")
		);
	}

	void parseAudioLoopName(const Value& elem,
		std::unordered_map<std::string, sf::Music::TimeSpan>& loopNames)
	{
		if (isValidString(elem, "name") == true)
		{
			loopNames[elem["name"sv].GetStringStr()] = parseAudioTimeSpan(elem);
		}
	}

	void parseAudioLoopNamesVal(const Value& elem, const char* key,
		std::unordered_map<std::string, sf::Music::TimeSpan>& loopNames)
	{
		const auto& loopElem = elem[key];

		if (loopElem.IsObject() == true)
		{
			parseAudioLoopName(loopElem, loopNames);
		}
		else if (loopElem.IsArray() == true)
		{
			for (const auto& val : loopElem)
			{
				parseAudioLoopName(val, loopNames);
			}
		}
	}

	void parseAudioLoopPoint(const rapidjson::Value& elem,
		std::vector<sf::Music::TimeSpan>& loopPoints)
	{
		loopPoints.push_back(parseAudioTimeSpan(elem));
	}

	void parseAudioLoopPointsVal(const Value& elem,
		const char* key, sf::MusicLoops& music)
	{
		std::vector<sf::Music::TimeSpan> loopPoints;

		const auto& loopElem = elem[key];

		if (loopElem.IsObject() == true)
		{
			parseAudioLoopPoint(loopElem, loopPoints);
		}
		else if (loopElem.IsArray() == true)
		{
			for (const auto& val : loopElem)
			{
				parseAudioLoopPoint(val, loopPoints);
			}
		}
		music.setMultiLoopPoints(loopPoints);
	}

	void updateAudioLoopString(const std::string_view str,
		const std::unordered_map<std::string, sf::Music::TimeSpan>& loopNames,
		sf::MusicLoops& music)
	{
		std::vector<sf::Music::TimeSpan> loopPoints;

		sf::Music::TimeSpan space;

		auto itSpace = loopNames.find(" ");
		if (itSpace != loopNames.cend())
		{
			space = itSpace->second;
		}

		auto strTokens = Utils::splitString(str, ' ');

		for (size_t i = 0; i < strTokens.size(); i++)
		{
			const auto& token = strTokens[i];

			if (token.find('_') != std::string::npos)
			{
				auto tokenTokens = Utils::splitString(str, '_');
				for (const auto& token2 : tokenTokens)
				{
					auto it = loopNames.find(token2);
					if (it != loopNames.cend())
					{
						loopPoints.push_back(it->second);
					}
				}
			}
			else
			{
				auto it = loopNames.find(token);
				if (it != loopNames.cend())
				{
					loopPoints.push_back(it->second);
				}
			}
			if (space.length.asMicroseconds() > 0 &&
				i < (strTokens.size() - 1))
			{
				loopPoints.push_back(space);
			}
		}
		music.setMultiLoopPoints(loopPoints);
	}
}
