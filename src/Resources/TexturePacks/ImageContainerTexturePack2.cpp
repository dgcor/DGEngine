#include "ImageContainerTexturePack2.h"
#include "Game/AnimationInfo.h"

uint32_t ImageContainerTexturePack2::getNormalizedDirection(uint32_t direction, uint32_t numberOfDirections)
{
	if (numberOfDirections == 8 ||
		numberOfDirections == 16)
	{
		switch (direction)
		{
		case 0:
			return 4;
		case 1:
			return 0;
		case 2:
			return 5;
		case 3:
			return 1;
		case 4:
			return 6;
		case 5:
			return 2;
		case 6:
			return 7;
		case 7:
			return 3;
		default:
			return direction;
		}
	}
	return direction;
}

ImageContainerTexturePack2::ImageContainerTexturePack2(const std::shared_ptr<ImageContainer>& imgPack_,
	const sf::Vector2f& offset_, const std::shared_ptr<Palette>& palette_, bool isIndexed_,
	bool normalizeDirections_) : ImageContainerTexturePack(imgPack_, offset_, palette_, isIndexed_),
	normalizeDirections(normalizeDirections_) {}

uint32_t ImageContainerTexturePack2::getDirection(uint32_t frameIdx) const noexcept
{
	auto numFrames = imgPack->size();
	if (frameIdx < numFrames)
	{
		auto directions = imgPack->getDirections();
		if (directions <= 1)
		{
			return 0;
		}
		auto framesPerDirection = numFrames / directions;
		if (normalizeDirections == false)
		{
			return frameIdx / framesPerDirection;
		}
		else
		{
			return getNormalizedDirection(
				frameIdx / framesPerDirection,
				directions
			);
		}
	}
	return 0;
}

AnimationInfo ImageContainerTexturePack2::getAnimation(int32_t groupIdx, int32_t directionIdx) const
{
	auto directions = imgPack->getDirections();
	if (normalizeDirections == true && directions > 1)
	{
		directionIdx = (int32_t)getNormalizedDirection(directionIdx, directions);
	}
	AnimationInfo animInfo;
	animInfo.indexRange = TexturePack::getRange(
		0,
		(uint32_t)cache.size(),
		directionIdx,
		directions
	);
	return animInfo;
}

int32_t ImageContainerTexturePack2::getFlags(uint32_t index, uint32_t subIndex) const
{
	auto levelFlags = dynamic_cast<const LevelFlags*>(imgPack.get());
	if (levelFlags != nullptr)
	{
		return levelFlags->getFlags(index, subIndex);
	}
	return 0;
}
