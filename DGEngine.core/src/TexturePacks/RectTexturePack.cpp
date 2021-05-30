#include "RectTexturePack.h"
#include "AnimationInfo.h"

void RectTexturePack::addRect(uint32_t index,
	const sf::IntRect& rect, const sf::Vector2f& offset)
{
	rects.push_back({ index, rect, offset });
}

void RectTexturePack::addRect(const sf::IntRect& rect, const sf::Vector2f& offset)
{
	rects.push_back({ (uint32_t)rects.size(), rect, offset });
}

void RectTexturePack::addGroup(uint32_t startIdx,
	uint32_t stopIdx, uint32_t directions, AnimationType animType)
{
	auto numFrames = stopIdx - startIdx;
	if (directions == 0 ||
		(directions > 0 && (numFrames % directions) != 0))
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

int32_t RectTexturePack::getWidth(uint32_t index) const
{
	return index < rects.size() ? rects[index].rect.width : 0;
}

uint32_t RectTexturePack::getGroupCount() const noexcept
{
	if (groups.size() > 0)
	{
		return (uint32_t)groups.size();
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
	if (groups.empty() == false)
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
	return texturePack->getDirection(frameIdx);
}

AnimationInfo RectTexturePack::getAnimation(int32_t groupIdx, int32_t directionIdx) const
{
	if (groupIdx >= 0 && (size_t)groupIdx < groups.size())
	{
		AnimationInfo animInfo;
		animInfo.indexRange = TexturePack::getRange(
			groups[groupIdx].startIdx,
			groups[groupIdx].stopIdx,
			directionIdx,
			groups[groupIdx].directions
		);
		animInfo.animType = groups[groupIdx].animType;
		return animInfo;
	}
	return texturePack->getAnimation(groupIdx, directionIdx);
}
