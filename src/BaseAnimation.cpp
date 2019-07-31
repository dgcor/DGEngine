#include "BaseAnimation.h"
#include "SFML/CompositeSprite.h"
#include "TextureInfo.h"

void BaseAnimation::setAnimation(int32_t groupIdx, int32_t directionIdx,
	bool resetAnimation, bool updateAnimationType)
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
		textureIndexRange = texturePackVar.getTexturePack()->getRange(
			groupIdx, directionIdx, at
		);
	}
	else
	{
		textureIndexRange = texturePackVar.getCompositeTexture()->getRange(
			groupIdx, directionIdx, at
		);
	}
	currentTextureIdx = textureIndexRange.first + currentRelativeIndex;
	if (updateAnimationType == true)
	{
		animType = at;
	}
	if (resetAnimation == true ||
		(resetAnimation == false && animType != AnimationType::BackAndForth))
	{
		backDirection = false;
	}
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
