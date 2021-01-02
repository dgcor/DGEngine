#include "BaseAnimation.h"
#include "SFML/CompositeSprite.h"
#include "TextureInfo.h"

BaseAnimation::BaseAnimation(TexturePackVariant texturePackVar_, bool pause_)
	: texturePackVar(std::move(texturePackVar_)), pause(pause_)
{
	setAnimation(-1, -1);
}

BaseAnimation::BaseAnimation(TexturePackVariant texturePackVar_,
	const AnimationInfo& animInfo, bool pause_)
	: texturePackVar(std::move(texturePackVar_)), textureIndexRange(animInfo.indexRange),
	currentTextureIdx(animInfo.indexRange.first), elapsedTime(animInfo.refresh),
	animType(animInfo.animType), pause(pause_) {}

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

void BaseAnimation::setTexturePack(TexturePackVariant texturePackVar_) noexcept
{
	texturePackVar = std::move(texturePackVar_);
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
	if (texturePackVar.holdsTexturePack() == true)
	{
		setAnimation(texturePackVar.getTexturePack()->getAnimation(groupIdx, directionIdx));
	}
	else if (texturePackVar.holdsCompositeTexture() == true)
	{
		setAnimation(texturePackVar.getCompositeTexture()->getAnimation(groupIdx, directionIdx));
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
	if (texturePackVar.holdsNullTexturePack() == true)
	{
		textureIndexRange = std::make_pair(0, 0);
		at = AnimationType::PlayOnce;
	}
	else if (texturePackVar.holdsTexturePack() == true)
	{
		auto animInfo = texturePackVar.getTexturePack()->getAnimation(
			groupIdx, directionIdx
		);
		textureIndexRange = animInfo.indexRange;
		at = animInfo.animType;
	}
	else
	{
		auto animInfo = texturePackVar.getCompositeTexture()->getAnimation(
			groupIdx, directionIdx
		);
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
	return texturePackVar.holdsNullTexturePack();
}

bool BaseAnimation::holdsTexturePack() const noexcept
{
	return texturePackVar.holdsTexturePack();
}

bool BaseAnimation::holdsTexturePack(const std::shared_ptr<TexturePack>& obj) const noexcept
{
	return texturePackVar.holdsTexturePack();
}

bool BaseAnimation::holdsCompositeTexture() const noexcept
{
	return texturePackVar.holdsCompositeTexture();
}

bool BaseAnimation::holdsCompositeTexture(const std::shared_ptr<CompositeTexture>& obj) const noexcept
{
	return texturePackVar.holdsCompositeTexture();
}

const std::shared_ptr<TexturePack>& BaseAnimation::getTexturePack() const
{
	return texturePackVar.getTexturePack();
}

std::shared_ptr<TexturePack>& BaseAnimation::getTexturePack()
{
	return texturePackVar.getTexturePack();
}

const std::shared_ptr<CompositeTexture>& BaseAnimation::getCompositeTexture() const
{
	return texturePackVar.getCompositeTexture();
}

std::shared_ptr<CompositeTexture>& BaseAnimation::getCompositeTexture()
{
	return texturePackVar.getCompositeTexture();
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
	if (texturePackVar.holdsNullTexturePack() == true)
	{
		return false;
	}
	else if (texturePackVar.holdsTexturePack() == true)
	{
		TextureInfo ti;
		auto texturePack = texturePackVar.getTexturePack().get();
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
		auto compTexture = texturePackVar.getCompositeTexture().get();
		if (compTexture->get(currentTextureIdx, ti) == true)
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
