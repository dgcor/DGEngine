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
	Animation(const std::shared_ptr<TexturePack>& texturePack, bool pause = false);

	Animation(const std::shared_ptr<TexturePack>& texturePack,
		const AnimationInfo& animInfo, bool pause = false);

	void updateTexture();

	bool Pause() const noexcept { return animation.pause; }
	void Pause(bool pause) noexcept { animation.pause = pause; }

	void setAnimation(const AnimationInfo& animInfo);
	void setAnimation(const std::shared_ptr<TexturePack>& texturePack, const AnimationInfo& animInfo);
	void setAnimation(int32_t groupIdx, int32_t directionIdx);

	void setAnimation(const std::shared_ptr<TexturePack>& texturePack,
		int32_t groupIdx, int32_t directionIdx);

	void setAnimation(int32_t groupIdx, int32_t directionIdx,
		bool resetAnimation, bool updateAnimationType);

	void setAnimation(const std::shared_ptr<TexturePack>& texturePack,
		int32_t groupIdx, int32_t directionIdx,
		bool resetAnimation, bool updateAnimationType);

	void setAnimation(int32_t groupIdx, int32_t directionIdx,
		AnimationType animationType);

	void setAnimation(const std::shared_ptr<TexturePack>& texturePack,
		int32_t groupIdx, int32_t directionIdx,
		AnimationType animationType);

	sf::Time getFrameTime() const noexcept { return animation.elapsedTime.timeout; }
	void setFrameTime(sf::Time time) noexcept { animation.elapsedTime.timeout = time; }

	void reset() noexcept { return animation.reset(); }
	bool isAnimationAtBeginning() const noexcept { return animation.isAnimationAtBeginning(); }
	bool isAnimationAtEnd() const noexcept { return animation.isAnimationAtEnd(); }
	bool hasPlayOnceAnimationFinished() const noexcept { return animation.hasPlayOnceAnimationFinished(); }

	bool holdsNullTexturePack() const noexcept;
	const std::shared_ptr<TexturePack>& getTexturePack() const;
	std::shared_ptr<TexturePack>& getTexturePack();

	void update(Game& game) override;

	bool getProperty(const std::string_view prop, Variable& var) const override;
};
