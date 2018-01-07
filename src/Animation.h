// Based on the Animation class for SFML by Maximilian Wagenbach (aka. Foaly) (foaly.f@web.de)
// https://github.com/SFML/SFML/wiki/Source:-AnimatedSprite

#pragma once

#include "Image.h"
#include <memory>
#include <SFML/System/Time.hpp>
#include "TexturePacks/TexturePack.h"

class Animation : public Image
{
private:
	std::shared_ptr<TexturePack> texturePack;
	size_t frameStart{ 0 };
	size_t frameEnd{ 0 };
	size_t frameIdx{ 0 };

	sf::Time frameTime;
	sf::Time currentTime;
	bool pause{ true };
	bool loop{ false };

	void updateTexture();
	void updateFrameIndex();

public:
	Animation(const sf::Texture& texture) : Image(texture) {}

	Animation(const std::shared_ptr<TexturePack>& texturePack_,
		size_t frameStart_, size_t frameEnd_,
		const sf::Time& frameTime = sf::milliseconds(50),
		bool pause_ = false, bool loop_ = true);

	sf::Time getFrameTime() const { return frameTime; }
	void setFrameTime(sf::Time time) { frameTime = time; }

	bool Pause() const { return pause; }
	void Pause(bool pause_) { pause = pause_; }
	bool Loop() const { return loop; }
	void Loop(bool loop_) { loop = loop_; }
	void stop()
	{
		pause = true;
		frameIdx = frameStart;
	}

	virtual void update(Game& game);
};
