#include "RectTexturePack.h"

void RectTexturePack::addRect(uint32_t index, const sf::IntRect& rect, const sf::Vector2f& offset)
{
	rects.push_back({ index, rect, offset });
}

void RectTexturePack::addRect(const sf::IntRect& rect, const sf::Vector2f& offset)
{
	rects.push_back({ (uint32_t)rects.size(), rect, offset });
}

bool RectTexturePack::get(uint32_t index, TextureInfo& ti) const
{
	if (rects.empty() == false)
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
	return texturePack->get(index, ti);
}

sf::Vector2i RectTexturePack::getTextureSize(uint32_t index) const
{
	if (rects.empty() == false)
	{
		return index < rects.size() ? rects[index].rect.getSize() : sf::Vector2i();
	}
	return texturePack->getTextureSize(index);
}

uint32_t RectTexturePack::size() const noexcept
{
	if (rects.empty() == false)
	{
		return (uint32_t)rects.size();
	}
	return texturePack->size();
}

std::pair<uint32_t, uint32_t> RectTexturePack::getDirection(uint32_t frameIdx, AnimationFlags& flags) const noexcept
{
	if (rects.empty() == true)
	{
		return texturePack->getDirection(frameIdx, flags);
	}
	flags = frameIdx < rects.size() ? AnimationFlags::Valid : AnimationFlags::Overflow;
	return {};
}

AnimationInfo RectTexturePack::getAnimation(int32_t groupIdx, int32_t directionIdx) const
{
	if (rects.empty() == true)
	{
		return texturePack->getAnimation(groupIdx, directionIdx);
	}
	AnimationInfo animInfo;
	animInfo.indexRange.first = 0;
	animInfo.indexRange.second = (uint32_t)rects.size() - 1;
	animInfo.flags = (groupIdx == 0 && directionIdx == 0) ? AnimationFlags::Valid : AnimationFlags::Overflow;
	return animInfo;
}
