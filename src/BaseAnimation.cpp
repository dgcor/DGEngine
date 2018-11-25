#include "BaseAnimation.h"

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

bool BaseAnimation::update(sf::Time elapsedTime) noexcept
{
	if (pause == true ||
		textureIndexRange.second <= textureIndexRange.first)
	{
		return false;
	}
	if (animType == AnimationType::PlayOnce &&
		currentTextureIdx >= textureIndexRange.second)
	{
		return false;
	}

	currentTime += elapsedTime;

	if (currentTime < frameTime)
	{
		return false;
	}
	do
	{
		currentTime -= frameTime;
		updateFrameIndex();
		if (animType == AnimationType::PlayOnce &&
			currentTextureIdx >= textureIndexRange.second)
		{
			break;
		}
	} while (currentTime >= frameTime);

	return true;
}
