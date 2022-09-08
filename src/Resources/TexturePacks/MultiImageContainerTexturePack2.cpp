#include "MultiImageContainerTexturePack2.h"
#include "Game/AnimationInfo.h"
#include "ImageContainerTexturePack2.h"

MultiImageContainerTexturePack2::MultiImageContainerTexturePack2(
	const std::vector<std::shared_ptr<ImageContainer>>& imgVec_, const sf::Vector2f& offset_,
	const std::shared_ptr<Palette>& palette_, bool isIndexed_, bool normalizeDirections_) :
	MultiImageContainerTexturePack(imgVec_, offset_, palette_, isIndexed_),
	normalizeDirections(normalizeDirections_) {}

uint32_t MultiImageContainerTexturePack2::getDirection(uint32_t frameIdx) const noexcept
{
	uint32_t startIdx = 0;
	for (const auto& imgPack : imgVec)
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
				return (frameIdx - startIdx) / framesPerDirection;
			}
			else
			{
				return ImageContainerTexturePack2::getNormalizedDirection(
					(frameIdx - startIdx) / framesPerDirection,
					directions
				);
			}
		}
		startIdx += numFrames;
	}
	return 0;
}

AnimationInfo MultiImageContainerTexturePack2::getAnimation(int32_t groupIdx, int32_t directionIdx) const
{
	AnimationInfo animInfo;
	if (groupIdx >= 0 && (uint32_t)groupIdx < imgVec.size())
	{
		uint32_t startIdx = 0;
		uint32_t stopIdx = 0;
		for (size_t i = 0; i < imgVec.size(); i++)
		{
			stopIdx += imgVec[i]->size();
			if (i == (size_t)groupIdx)
			{
				auto directions = imgVec[i]->getDirections();
				if (normalizeDirections == true && directions > 1)
				{
					directionIdx = (int32_t)ImageContainerTexturePack2::getNormalizedDirection(
						directionIdx, directions
					);
				}
				animInfo.indexRange = TexturePack::getRange(
					startIdx,
					stopIdx,
					directionIdx,
					directions
				);
				return animInfo;
			}
			startIdx = stopIdx;
		}
	}
	animInfo.indexRange = std::make_pair((uint32_t)0, textureCount - 1);
	return animInfo;
}
