// Based on the Animation class for SFML by Maximilian Wagenbach (aka. Foaly) (foaly.f@web.de)
// https://github.com/SFML/SFML/wiki/Source:-AnimatedSprite

#pragma once

#include "AnimationInfo.h"
#include <cstddef>
#include "TexturePacks/CompositeTexturePack.h"
#include <utility>
#include "Utils/ElapsedTime.h"

class CompositeSprite;

class BaseAnimation
{
private:
	std::shared_ptr<TexturePack> texturePack;
	const CompositeTexturePack* compositeTexturePack{ nullptr };

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
	BaseAnimation(const std::shared_ptr<TexturePack>& texturePack_, bool pause_);

	BaseAnimation(const std::shared_ptr<TexturePack>& texturePack_,
		const AnimationInfo& animInfo, bool pause_);

	void clear() noexcept;
	void reset() noexcept;

	void setTexturePack(const std::shared_ptr<TexturePack>& texturePack_) noexcept;

	void setAnimation(const AnimationInfo& animInfo) noexcept;
	void setAnimation(int32_t groupIdx, int32_t directionIdx) noexcept;

	void setAnimation(int32_t groupIdx, int32_t directionIdx,
		bool resetAnimation, AnimationType animationType);

	void setAnimation(int32_t groupIdx, int32_t directionIdx,
		bool resetAnimation, bool updateAnimationType);

	void setAnimation(int32_t groupIdx, int32_t directionIdx,
		AnimationType animationType);

	bool isAnimationAtBeginning() const noexcept;
	bool isAnimationAtEnd() const noexcept;
	bool hasPlayOnceAnimationFinished() const noexcept;

	bool holdsNullTexturePack() const noexcept;
	bool holdsCompositeTexturePack() const noexcept;
	const std::shared_ptr<TexturePack>& getTexturePack() const;
	std::shared_ptr<TexturePack>& getTexturePack();

	bool update(sf::Time elapsedTime_) noexcept;

	bool updateTexture(CompositeSprite& sprite, bool& absoluteOffset) const;
	bool updateTexture(CompositeSprite& sprite) const;
};
