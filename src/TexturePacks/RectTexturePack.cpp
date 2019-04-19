#include "RectTexturePack.h"
#include "TextureInfo.h"

void RectTexturePack::addRect(size_t index,
	const sf::IntRect& rect, const sf::Vector2f& offset)
{
	rects.push_back({ index, rect, offset });
}

bool RectTexturePack::get(size_t index, TextureInfo& ti) const
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
