#include "BaseAnimation.h"
#include "SFML/CompositeSprite.h"

BaseAnimation::BaseAnimation(const std::shared_ptr<TexturePack>& texturePack_, bool pause_)
	: texturePack(texturePack_), pause(pause_)
{
	compositeTexturePack = dynamic_cast<const CompositeTexturePack*>(texturePack.get());
	setAnimation(-1, -1);
}

BaseAnimation::BaseAnimation(const std::shared_ptr<TexturePack>& texturePack_,
	const AnimationInfo& animInfo, bool pause_)
	: texturePack(texturePack_), textureIndexRange(animInfo.indexRange),
	currentTextureIdx(animInfo.indexRange.first), elapsedTime(animInfo.refresh),
	animType(animInfo.animType), pause(pause_)
{
	compositeTexturePack = dynamic_cast<const CompositeTexturePack*>(texturePack.get());
}

void BaseAnimation::clear() noexcept
{
	textureIndexRange.first = 0;
	textureIndexRange.second = 0;
	elapsedTime.reset();
	backDirection = false;
}

void BaseAnimation::reset() noexcept
{
	currentTextureIdx = textureIndexRange.first;
	elapsedTime.reset();
	backDirection = false;
}

void BaseAnimation::setTexturePack(const std::shared_ptr<TexturePack>& texturePack_) noexcept
{
	texturePack = texturePack_;
	compositeTexturePack = dynamic_cast<const CompositeTexturePack*>(texturePack.get());
}

void BaseAnimation::setAnimation(const AnimationInfo& animInfo) noexcept
{
	textureIndexRange = animInfo.indexRange;
	currentTextureIdx = textureIndexRange.first;
	elapsedTime = animInfo.refresh;
	animType = animInfo.animType;
	backDirection = false;
}

void BaseAnimation::setAnimation(int32_t groupIdx, int32_t directionIdx) noexcept
{
	if (texturePack != nullptr)
	{
		setAnimation(texturePack->getAnimation(groupIdx, directionIdx));
	}
}

void BaseAnimation::setAnimation(int32_t groupIdx, int32_t directionIdx,
	bool resetAnimation, AnimationType animationType)
{
	AnimationType at = {};
	uint32_t currentRelativeIndex = 0;
	if (resetAnimation == false)
	{
		currentRelativeIndex = currentTextureIdx - textureIndexRange.first;
	}
	if (texturePack == nullptr)
	{
		textureIndexRange = std::make_pair(0, 0);
		at = AnimationType::PlayOnce;
	}
	else
	{
		auto animInfo = texturePack->getAnimation(groupIdx, directionIdx);
		textureIndexRange = animInfo.indexRange;
		at = animInfo.animType;
	}
	currentTextureIdx = textureIndexRange.first + currentRelativeIndex;
	animType = animationType != AnimationType::Size ? animationType : at;
	if (resetAnimation == true ||
		(resetAnimation == false && animType != AnimationType::BackAndForth))
	{
		backDirection = false;
	}
}

void BaseAnimation::setAnimation(int32_t groupIdx, int32_t directionIdx,
	bool resetAnimation, bool updateAnimationType)
{
	auto animationType = updateAnimationType ? AnimationType::Size : animType;
	setAnimation(groupIdx, directionIdx, true, animationType);
}

void BaseAnimation::setAnimation(int32_t groupIdx, int32_t directionIdx,
	AnimationType animationType)
{
	setAnimation(groupIdx, directionIdx, true, animationType);
}

void BaseAnimation::updateFrameIndex() noexcept
{
	if (animType != AnimationType::BackAndForth)
	{
		if (currentTextureIdx < textureIndexRange.second)
		{
			currentTextureIdx++;
		}
		else
		{
			currentTextureIdx = textureIndexRange.first;
		}
	}
	else
	{
		if (backDirection == false)
		{
			if (currentTextureIdx < textureIndexRange.second)
			{
				currentTextureIdx++;
			}
			if (currentTextureIdx == textureIndexRange.second)
			{
				backDirection = true;
			}
		}
		else
		{
			if (currentTextureIdx > textureIndexRange.first)
			{
				currentTextureIdx--;
			}
			if (currentTextureIdx == textureIndexRange.first)
			{
				backDirection = false;
			}
		}
	}
}

bool BaseAnimation::isAnimationAtBeginning() const noexcept
{
	return currentTextureIdx <= textureIndexRange.first;
}

bool BaseAnimation::isAnimationAtEnd() const noexcept
{
	return currentTextureIdx >= textureIndexRange.second;
}

bool BaseAnimation::hasPlayOnceAnimationFinished() const noexcept
{
	return animType == AnimationType::PlayOnce &&
		currentTextureIdx >= textureIndexRange.second;
}

bool BaseAnimation::holdsNullTexturePack() const noexcept
{
	return texturePack == nullptr;
}

bool BaseAnimation::holdsCompositeTexturePack() const noexcept
{
	return compositeTexturePack != nullptr;
}

const std::shared_ptr<TexturePack>& BaseAnimation::getTexturePack() const
{
	return texturePack;
}

std::shared_ptr<TexturePack>& BaseAnimation::getTexturePack()
{
	return texturePack;
}

bool BaseAnimation::update(sf::Time elapsedTime_) noexcept
{
	if (pause == true ||
		textureIndexRange.second <= textureIndexRange.first)
	{
		return false;
	}
	if (hasPlayOnceAnimationFinished() == true)
	{
		return false;
	}
	if (elapsedTime.timeout == sf::Time::Zero)
	{
		return false;
	}

	elapsedTime.update(elapsedTime_, [&]() -> bool
	{
		updateFrameIndex();
		if (hasPlayOnceAnimationFinished() == true)
		{
			return false;
		}
		return true;
	});

	return true;
}

bool BaseAnimation::updateTexture(CompositeSprite& sprite, bool& absoluteOffset) const
{
	if (texturePack == nullptr)
	{
		return false;
	}
	else if (compositeTexturePack == nullptr)
	{
		TextureInfo ti;
		if (texturePack->get(currentTextureIdx, ti) == true)
		{
			sprite.setTexture(ti);
			absoluteOffset = ti.absoluteOffset;
			return true;
		}
	}
	else
	{
		std::vector<TextureInfo> ti;
		if (compositeTexturePack->get(currentTextureIdx, ti) == true)
		{
			sprite.setTexture(ti);
			absoluteOffset = ti.front().absoluteOffset;
			return true;
		}
	}
	return false;
}

bool BaseAnimation::updateTexture(CompositeSprite& sprite) const
{
	bool absoluteoffet;
	return updateTexture(sprite, absoluteoffet);
}
