#pragma once

#include <memory>
#include <SFML/Audio/Music.hpp>
#include <SFML/System/NonCopyable.hpp>
#include "PhysFSStream.h"

class Music2 : public sf::NonCopyable
{
private:
	std::unique_ptr<sf::PhysFSStream> file;
	std::unique_ptr<sf::Music> music;

public:
	Music2(const std::string& file_) : Music2(file_.c_str()) {}
	Music2(const char* file_) : file(std::make_unique<sf::PhysFSStream>(file_)),
		music(std::make_unique<sf::Music>()) {}

	bool load()
	{
		if (file == nullptr || file->hasError() == true)
		{
			return false;
		}
		return music->openFromStream(*file);
	}

	void play() { music->play(); }
	void pause() { music->pause(); }
	void stop() { music->stop(); }

	unsigned int getChannelCount() const { return music->getChannelCount(); }
	bool getLoop() const { return music->getLoop(); }
	sf::Time getPlayingOffset() const { return music->getPlayingOffset(); }
	unsigned int getSampleRate() const { return music->getSampleRate(); }
	sf::Music::Status getStatus() const { return music->getStatus(); }

	void setLoop(bool loop) { music->setLoop(loop); }
	void setPlayingOffset(sf::Time timeOffset) { music->setPlayingOffset(timeOffset); }
	void setVolume(float volume) { music->setVolume(volume); }
};
