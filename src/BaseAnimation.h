// Based on the Animation class for SFML by Maximilian Wagenbach (aka. Foaly) (foaly.f@web.de)
// https://github.com/SFML/SFML/wiki/Source:-AnimatedSprite

#pragma once

#include "AnimationInfo.h"
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
	BaseAnimation(TexturePackVariant texturePackVar_, bool pause_);

	BaseAnimation(TexturePackVariant texturePackVar_,
		const AnimationInfo& animInfo, bool pause_);

	void clear() noexcept;
	void reset() noexcept;

	void setTexturePack(TexturePackVariant texturePackVar_) noexcept;

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
	bool holdsTexturePack() const noexcept;
	bool holdsTexturePack(const std::shared_ptr<TexturePack>& obj) const noexcept;
	bool holdsCompositeTexture() const noexcept;
	bool holdsCompositeTexture(const std::shared_ptr<CompositeTexture>& obj) const noexcept;
	const std::shared_ptr<TexturePack>& getTexturePack() const;
	std::shared_ptr<TexturePack>& getTexturePack();
	const std::shared_ptr<CompositeTexture>& getCompositeTexture() const;
	std::shared_ptr<CompositeTexture>& getCompositeTexture();

	bool update(sf::Time elapsedTime_) noexcept;

	bool updateTexture(CompositeSprite& sprite, bool& absoluteOffset) const;
	bool updateTexture(CompositeSprite& sprite) const;
};
