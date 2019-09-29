#pragma once

#include "BaseAnimation.h"
#include "Image.h"

class Animation : public Image
{
protected:
	BaseAnimation animation;

public:
	Animation() {}

	Animation(const sf::Texture& texture) : Image(texture) {}

	Animation(TexturePackVariant texturePackVar_,
		const std::pair<size_t, size_t>& textureIndexRange,
		AnimationType type = AnimationType::Looped, bool pause = false);

	bool Pause() const noexcept { return animation.pause; }
	void Pause(bool pause) noexcept { animation.pause = pause; }

	void setAnimation(TexturePackVariant texturePackVar_,
		int32_t groupIdx, int32_t directionIdx,
		bool resetAnimation, bool updateAnimationType) noexcept;

	void setAnimation(int32_t groupIdx, int32_t directionIdx,
		bool resetAnimation, bool updateAnimationType) noexcept;

	sf::Time getFrameTime() const noexcept { return animation.elapsedTime.timeout; }
	void setFrameTime(sf::Time time) noexcept { animation.elapsedTime.timeout = time; }

	void update(Game& game) override;

	bool getProperty(const std::string_view prop, Variable& var) const override;
};
