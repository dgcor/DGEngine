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

	sf::SoundBuffer* parseSoundLoopsObj(Game& game, const rapidjson::Value& elem,
		const std::string& id, const std::string& file);

	sf::SoundBuffer* parseSingleSoundObj(Game& game, const rapidjson::Value& elem);

	sf::SoundBuffer* parseMultiSoundObj(Game& game, const std::string& id,
		const std::string_view resource, const sf::Int16* samples,
		sf::Uint64 sampleCount, unsigned channelCount, unsigned sampleRate);

	sf::SoundBuffer* parseMultiSoundLoopsObj(Game& game, const rapidjson::Value& elem,
		const std::string& id, const sf::Int16* samples,
		sf::Uint64 sampleCount, unsigned channelCount, unsigned sampleRate);

	sf::SoundBuffer* parseMultiSoundObj(Game& game, const rapidjson::Value& elem);

	bool parseSoundFromId(Game& game, const rapidjson::Value& elem);

	sf::SoundBuffer* getSoundObj(Game& game, const rapidjson::Value& elem);

	typedef decltype(&getSoundObj) getSoundObjFuncPtr;

	void parseSound(Game& game, const rapidjson::Value& elem,
		const getSoundObjFuncPtr getSoundObjFunc);

	void parseSound(Game& game, const rapidjson::Value& elem);
}
