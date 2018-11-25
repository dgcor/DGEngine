// Based on the Animation class for SFML by Maximilian Wagenbach (aka. Foaly) (foaly.f@web.de)
// https://github.com/SFML/SFML/wiki/Source:-AnimatedSprite

#pragma once

#include <stddef.h>
#include "AnimationType.h"
#include <SFML/System/Time.hpp>
#include <utility>

class BaseAnimation
{
public:
	std::pair<size_t, size_t> textureIndexRange;
	size_t currentTextureIdx{ 0 };

	sf::Time frameTime{ sf::milliseconds(50) };
	sf::Time currentTime;
	AnimationType animType{ AnimationType::PlayOnce };
	bool pause{ false };
	bool backDirection{ false };

private:
	void updateFrameIndex() noexcept;

public:
	BaseAnimation() {}
	BaseAnimation(const std::pair<size_t, size_t>& textureIndexRange_,
		const sf::Time& frameTime_, AnimationType animType_, bool pause_)
		: textureIndexRange(textureIndexRange_), currentTextureIdx(textureIndexRange_.first),
		frameTime(frameTime_), animType(animType_), pause(pause_) {}

	void clear() noexcept
	{
		textureIndexRange.first = 0;
		textureIndexRange.second = 0;
		backDirection = false;
	}
	void reset() noexcept
	{
		currentTextureIdx = textureIndexRange.first;
		backDirection = false;
	}

	bool update(sf::Time elapsedTime) noexcept;
};
