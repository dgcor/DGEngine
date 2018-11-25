#pragma once

#include <memory>
#include "PhysFSStream.h"
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <unordered_map>
#include <variant>

struct SoundBufferLoops
{
	sf::SoundBuffer soundBuffer;
	std::unordered_map<std::string, sf::Music::TimeSpan> loops;
};

struct SoundFileLoops
{
	sf::PhysFSStream file;
	std::unordered_map<std::string, sf::Music::TimeSpan> loops;

	SoundFileLoops(const std::string& fileName) : file(fileName) {}
	SoundFileLoops(const char* fileName) : file(fileName) {}
};

typedef std::variant<
	std::shared_ptr<sf::SoundBuffer>,
	std::shared_ptr<sf::PhysFSStream>,
	std::shared_ptr<SoundBufferLoops>,
	std::shared_ptr<SoundFileLoops>>
	AudioSource;
