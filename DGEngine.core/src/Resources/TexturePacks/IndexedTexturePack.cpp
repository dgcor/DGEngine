#include "IndexedTexturePack.h"
#include "Game/AnimationInfo.h"
#include <ranges>

IndexedTexturePack::IndexedTexturePack(std::unique_ptr<TexturePack> texturePack_, bool onlyUseIndexed_)
	: TexturePackIndexWrapper(std::move(texturePack_)), onlyUseIndexed(onlyUseIndexed_)
{
	if (onlyUseIndexed == false)
	{
		indexRange = texturePack->getAnimation(-1, -1).indexRange;
	}
}

void IndexedTexturePack::mapTextureIndex(uint32_t mapIndex, bool reverseMap)
{
	if (reverseMap == false)
	{
		mapTextureIndex(mapIndex, (uint32_t)textureIndexes.size());
	}
	else
	{
		mapTextureIndex((uint32_t)textureIndexes.size(), mapIndex);
	}
}

void IndexedTexturePack::mapTextureIndex(uint32_t mapIndex, uint32_t toIndex)
{
	if (onlyUseIndexed == true &&
		textureIndexes.empty() == true)
	{
		indexRange.first = mapIndex;
		indexRange.second = mapIndex;
	}
	if (indexRange.first > mapIndex)
	{
		indexRange.first = mapIndex;
	}
	if (indexRange.second < mapIndex)
	{
		indexRange.second = mapIndex;
	}
	textureIndexes[mapIndex] = toIndex;
}

void IndexedTexturePack::mapTextureIndexRange(const PairInt32& indexRange,
	int32_t indexRangeStart, bool reverseMap)
{
	int64_t startIndex = indexRange.x;
	int64_t stopIndex = indexRange.y;
	if (indexRangeStart < 0)
	{
		if (startIndex <= stopIndex)
		{
			for (auto idx : std::views::iota(startIndex, stopIndex + 1))
			{
				mapTextureIndex((uint32_t)idx, reverseMap);
			}
		}
		else
		{
			for (auto idx : std::views::iota(stopIndex, startIndex + 1) | std::views::reverse)
			{
				mapTextureIndex((uint32_t)idx, reverseMap);
			}
		}
	}
	else
	{
		if (startIndex <= stopIndex)
		{
			uint32_t idxMapTo = 0;
			for (auto idx : std::views::iota(startIndex, stopIndex + 1))
			{
				mapTextureIndex((uint32_t)idx, (uint32_t)(indexRangeStart + idxMapTo++));
			}
		}
		else
		{
			uint32_t idxMapTo = 0;
			for (auto idx : std::views::iota(stopIndex, startIndex + 1) | std::views::reverse)
			{
				mapTextureIndex((uint32_t)idx, (uint32_t)(indexRangeStart + idxMapTo++));
			}
		}
	}
}

bool IndexedTexturePack::translateIndex(uint32_t& index) const
{
	auto it = textureIndexes.find(index);
	if (it != textureIndexes.cend())
	{
		index = it->second;
	}
	else if (onlyUseIndexed == true)
	{
		return false;
	}
	return true;
}

uint32_t IndexedTexturePack::size() const noexcept
{
	if (onlyUseIndexed == true)
	{
		return (uint32_t)textureIndexes.size();
	}
	return indexRange.second + 1 - indexRange.first;
}

uint32_t IndexedTexturePack::getGroupCount() const noexcept
{
	if (onlyUseIndexed == false)
	{
		return texturePack->getGroupCount();
	}
	return 1;
}

uint32_t IndexedTexturePack::getDirectionCount(uint32_t groupIdx) const noexcept
{
	if (onlyUseIndexed == false)
	{
		return texturePack->getDirectionCount(groupIdx);
	}
	return 1;
}

std::pair<uint32_t, uint32_t> IndexedTexturePack::getDirection(uint32_t frameIdx, AnimationFlags& flags) const noexcept
{
	if (onlyUseIndexed == false)
	{
		auto groupDirection = texturePack->getDirection(frameIdx, flags);
		if ((int)(flags & AnimationFlags::Valid) != 0)
		{
			return groupDirection;
		}
	}
	if (frameIdx >= indexRange.first && frameIdx <= indexRange.second)
	{
		flags = AnimationFlags::Valid;
	}
	else
	{
		flags = AnimationFlags::Overflow;
	}
	return {};
}

AnimationInfo IndexedTexturePack::getAnimation(int32_t groupIdx, int32_t directionIdx) const
{
	if (onlyUseIndexed == false)
	{
		auto animInfo = texturePack->getAnimation(groupIdx, directionIdx);
		if (animInfo.Valid() == true)
		{
			return animInfo;
		}
	}
	AnimationInfo animInfo;
	animInfo.indexRange = indexRange;
	animInfo.flags = (groupIdx == 0 && directionIdx == 0) ? AnimationFlags::Valid : AnimationFlags::Overflow;
	return animInfo;
}
