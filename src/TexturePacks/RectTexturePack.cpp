#include "RectTexturePack.h"
#include "TextureInfo.h"

void RectTexturePack::addRect(uint32_t index,
	const sf::IntRect& rect, const sf::Vector2f& offset)
{
	rects.push_back({ index, rect, offset });
}

void RectTexturePack::addGroup(uint32_t startIdx,
	uint32_t stopIdx, uint32_t directions, AnimationType animType)
{
	auto numFrames = stopIdx - startIdx;
	if (directions > 0 && (numFrames % directions) != 0)
	{
		directions = 1;
	}
	groups.push_back({ startIdx, stopIdx, directions, animType });
}

bool RectTexturePack::get(uint32_t index, TextureInfo& ti) const
{
	if (index < rects.size() &&
		texturePack->get(rects[index].index, ti) == true)
	{
		ti.textureRect = rects[index].rect;
		ti.offset = rects[index].offset;
		ti.absoluteOffset = absoluteOffsets;
		return true;
	}
	return false;
}

uint32_t RectTexturePack::getGroupCount() const noexcept
{
	if (groups.size() > 0)
	{
		return groups.size();
	}
	return texturePack->getGroupCount();
}

uint32_t RectTexturePack::getDirectionCount(uint32_t groupIdx) const noexcept
{
	if (groupIdx >= 0 && (size_t)groupIdx < groups.size())
	{
		return groups[groupIdx].directions;
	}
	return texturePack->getDirectionCount(groupIdx);
}

uint32_t RectTexturePack::getDirection(uint32_t frameIdx) const noexcept
{
	for (const auto& group : groups)
	{
		if (frameIdx >= group.startIdx && frameIdx < group.stopIdx)
		{
			if (group.directions <= 1)
			{
				return 0;
			}
			auto numFrames = group.stopIdx - group.startIdx;
			auto framesPerDirection = numFrames / group.directions;
			return (frameIdx - group.startIdx) / framesPerDirection;
		}
	}
	return 0;
}

std::pair<uint32_t, uint32_t> RectTexturePack::getRange(
	int32_t groupIdx, int32_t directionIdx, AnimationType& animType) const
{
	if (groupIdx >= 0 && (size_t)groupIdx < groups.size())
	{
		animType = groups[groupIdx].animType;
		return TexturePack::getRange(
			groups[groupIdx].startIdx,
			groups[groupIdx].stopIdx,
			directionIdx,
			groups[groupIdx].directions
		);
	}
	animType = {};
	return std::make_pair((uint32_t)0, (uint32_t)(rects.size() - 1));
}
