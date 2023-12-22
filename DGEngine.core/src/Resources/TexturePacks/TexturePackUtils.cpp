#include "TexturePackUtils.h"

uint32_t TexturePackUtils::calculateDirection(uint32_t frameIdx, uint32_t startIdx,
	uint32_t numFrames, uint32_t directions, AnimationFlags& flags)
{
	if (frameIdx >= startIdx && frameIdx < startIdx + numFrames)
	{
		if (directions == 0)
		{
			flags = AnimationFlags::Overflow;
			return 0;
		}
		flags = AnimationFlags::Valid;
		if (directions == 1)
		{
			return 0;
		}
		if (numFrames <= directions)
		{
			return (frameIdx - startIdx);
		}
		return ((frameIdx - startIdx) * directions) / numFrames;
	}
	flags = AnimationFlags::Overflow;
	return 0;
}

bool TexturePackUtils::calculateRange(uint32_t startIdx, uint32_t stopIdx,
	int32_t directionIdx, uint32_t directions, AnimationInfo& animInfo)
{
	if (startIdx <= stopIdx && directionIdx >= 0)
	{
		if (directionIdx == 0 && directions == 1)
		{
			animInfo.flags = AnimationFlags::Valid;
			animInfo.indexRange = { startIdx, stopIdx };
			return true;
		}

		auto numFrames = stopIdx - startIdx + 1;
		if (directions > numFrames)
		{
			directions = numFrames;
		}
		if (directions > 0 && (uint32_t)directionIdx < directions)
		{
			auto frameSize = numFrames / directions;
			auto rangeStart = startIdx + (frameSize * directionIdx);
			if (numFrames % directions == 0)
			{
				animInfo.indexRange = std::make_pair(
					rangeStart,
					startIdx + (frameSize * (directionIdx + 1)) - 1
				);
			}
			else
			{
				auto frameIdx = startIdx;
				bool hasStart = false;
				bool hasStop = false;
				do
				{
					AnimationFlags dirFlags;
					auto calculatedDirection = calculateDirection(frameIdx, startIdx, numFrames, directions, dirFlags);
					if (hasStart == false && calculatedDirection == (uint32_t)directionIdx)
					{
						animInfo.indexRange.first = frameIdx;
						animInfo.indexRange.second = frameIdx;
						hasStart = true;
					}
					else if (hasStop == false)
					{
						if (calculatedDirection == (uint32_t)directionIdx)
						{
							animInfo.indexRange.second = frameIdx;
						}
						else if (hasStart == true)
						{
							break;
						}
					}
					frameIdx++;
				} while (true);
			}
			animInfo.flags = AnimationFlags::Valid;
			return true;
		}
	}
	animInfo.flags = AnimationFlags::Overflow;
	animInfo.indexRange = { startIdx, stopIdx };
	return false;
}
