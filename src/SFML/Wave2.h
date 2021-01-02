#pragma once

#define DR_WAV_NO_STDIO
#include "dr_wav.h"
#include <SFML/Audio/SoundFileReader.hpp>

// Wave file reader using a different decoder from SFML's default.
class Wave2FileReader : public sf::SoundFileReader
{
private:
	drwav wave{};
	bool isInitialized{ false };

public:
	Wave2FileReader() = default;
	~Wave2FileReader() override;

	static bool check(sf::InputStream& stream_);

	bool open(sf::InputStream& stream_, Info& info) override;
	void seek(sf::Uint64 sampleOffset) override;
	sf::Uint64 read(sf::Int16* samples, sf::Uint64 maxCount) override;
};
