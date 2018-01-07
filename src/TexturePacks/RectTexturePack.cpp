#include "RectTexturePack.h"

void RectTexturePack::add(const sf::IntRect& rect)
{
	if (rects.empty() == true)
	{
		texturesHaveSameSize = true;
	}
	else if (texturesHaveSameSize == true && (
		rect.width != rects.back().width ||
		rect.height != rects.back().height))
	{
		texturesHaveSameSize = false;
	}
	rects.push_back(rect);
}

bool RectTexturePack::get(size_t index,
	const sf::Texture** textureOut, sf::IntRect& textureRect) const
{
	if (index < rects.size())
	{
		textureRect = rects[index];
		(*textureOut) = texture.get();
		return true;
	}
	return false;
}

bool RectTexturePack::get(size_t indexX, size_t indexY,
	const sf::Texture** textureOut, sf::IntRect& textureRect) const
{
	return get(indexX, textureOut, textureRect);
}

bool RectTexturePack::getTextureSize(sf::Vector2i& textureSize) const
{
	if (texturesHaveSameSize == false)
	{
		return false;
	}
	textureSize.x = rects.front().width;
	textureSize.y = rects.front().height;
	return true;
}
