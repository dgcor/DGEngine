#pragma once

#include <memory>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "SFML/PhysFSStream.h"
#include "Utils/UnorderedStringMap.h"
#include <variant>

struct SoundBufferLoops
{
	sf::SoundBuffer soundBuffer;
	UnorderedStringMap<sf::Music::TimeSpan> loops;
};

struct SoundFileLoops
{
	sf::PhysFSStream file;
	UnorderedStringMap<sf::Music::TimeSpan> loops;

	SoundFileLoops(const std::string& fileName) : file(fileName) {}
	SoundFileLoops(const char* fileName) : file(fileName) {}
};

using AudioSource = std::variant<
	std::shared_ptr<sf::SoundBuffer>,
	std::shared_ptr<sf::InputStream>,
	std::shared_ptr<SoundBufferLoops>,
	std::shared_ptr<SoundFileLoops>>;
