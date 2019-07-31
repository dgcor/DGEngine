// Based on the Animation class for SFML by Maximilian Wagenbach (aka. Foaly) (foaly.f@web.de)
// https://github.com/SFML/SFML/wiki/Source:-AnimatedSprite

#pragma once

#include "AnimationType.h"
#include "CompositeTexture.h"
#include <cstddef>
#include <utility>
#include "Utils/ElapsedTime.h"
#include <variant>

class CompositeSprite;

class BaseAnimation
{
private:
	TexturePackVariant texturePackVar;

public:
	std::pair<uint32_t, uint32_t> textureIndexRange;
	uint32_t currentTextureIdx{ 0 };

	ElapsedTime elapsedTime{ sf::milliseconds(50) };
	AnimationType animType{ AnimationType::PlayOnce };
	bool pause{ false };
	bool backDirection{ false };

private:
	void updateFrameIndex() noexcept;

public:
	BaseAnimation() {}

	BaseAnimation(TexturePackVariant texturePackVar_,
		const std::pair<uint32_t, uint32_t>& textureIndexRange_,
		const sf::Time& frameTime_, AnimationType animType_, bool pause_)
		: texturePackVar(texturePackVar_), textureIndexRange(textureIndexRange_),
		currentTextureIdx(textureIndexRange_.first), elapsedTime(frameTime_),
		animType(animType_), pause(pause_) {}

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

	void setTexturePack(TexturePackVariant texturePackVar_) noexcept
	{
		texturePackVar = std::move(texturePackVar_);
	}

	void setAnimation(int32_t groupIdx, int32_t directionIdx,
		bool resetAnimation, bool updateAnimationType);

	constexpr bool holdsNullTexturePack() const noexcept
	{
		return texturePackVar.holdsNullTexturePack();
	}

	bool isAnimationAtBeginning() const noexcept;
	bool isAnimationAtEnd() const noexcept;

	bool hasPlayOnceAnimationFinished() const noexcept;

	bool update(sf::Time elapsedTime_) noexcept;

	bool updateTexture(CompositeSprite& sprite, bool& absoluteOffset) const;
	bool updateTexture(CompositeSprite& sprite) const;
};
