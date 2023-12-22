#include "AnimationGroup.h"
#include "TexturePackUtils.h"

uint32_t AnimationGroup::getDirection(uint32_t frameIdx, AnimationFlags& flags) const noexcept
{
	if (directionsVec.empty() == false)
	{
		for (uint32_t i = 0; i < directionsVec.size(); i++)
		{
			const auto& range = directionsVec[i];
			auto idx = frameIdx - indexRange.first;
			if (idx >= range.first && idx <= range.second)
			{
				flags = AnimationFlags::Valid;
				return i;
			}
		}
		flags = AnimationFlags::Overflow;
		return 0;
	}
	return TexturePackUtils::calculateDirection(frameIdx, indexRange.first, size(), directions, flags);
}

AnimationInfo AnimationGroup::getAnimation(int32_t groupIdx, int32_t directionIdx) const
{
	AnimationInfo animInfo;
	animInfo.animType = animType;
	animInfo.refresh = refresh;
	if (groupIdx == 0 && directionIdx >= 0)
	{
		if (directionsVec.empty() == false)
		{
			if ((uint32_t)directionIdx < directionsVec.size())
			{
				animInfo.indexRange = directionsVec[directionIdx];
				if (indexRange.first > 0)
				{
					animInfo.indexRange.first += indexRange.first;
					animInfo.indexRange.second += indexRange.first;
				}
				animInfo.flags = AnimationFlags::Valid;
				return animInfo;
			}
		}
		else if ((uint32_t)directionIdx < directions)
		{
			TexturePackUtils::calculateRange(
				indexRange.first,
				indexRange.second,
				directionIdx,
				directions,
				animInfo
			);
			return animInfo;
		}
	}
	animInfo.indexRange = indexRange;
	animInfo.flags = AnimationFlags::Overflow;
	return animInfo;
}
