// Based on the Animation class for SFML by Maximilian Wagenbach (aka. Foaly) (foaly.f@web.de)
// https://github.com/SFML/SFML/wiki/Source:-AnimatedSprite

#pragma once

#include "AnimationType.h"
#include <cstddef>
#include <utility>
#include "Utils/ElapsedTime.h"

class BaseAnimation
{
public:
	std::pair<size_t, size_t> textureIndexRange;
	size_t currentTextureIdx{ 0 };

	ElapsedTime elapsedTime{ sf::milliseconds(50) };
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
		elapsedTime(frameTime_), animType(animType_), pause(pause_) {}

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

	bool update(sf::Time elapsedTime_) noexcept;
};
