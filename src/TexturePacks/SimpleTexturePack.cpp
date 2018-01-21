#include "SimpleTexturePack.h"

static bool ConstructTexturePack(MultiTexture& t, const std::shared_ptr<sf::Texture>& texture,
	size_t xFrames, size_t yFrames, bool horizontalDirection)
{
	if (texture == nullptr)
	{
		return false;
	}
	t.texture = texture;
	t.horizontalDirection = horizontalDirection;
	t.numFrames = xFrames * yFrames;
	if (t.numFrames == 0)
	{
		return false;
	}

	auto imgSize = texture->getSize();

	if (imgSize.x == 0 ||
		imgSize.y == 0 ||
		xFrames > imgSize.x ||
		yFrames > imgSize.y ||
		imgSize.x % xFrames != 0 ||
		imgSize.y % yFrames != 0)
	{
		t.numFrames = 0;
		return false;
	}

	t.maxFrames = (horizontalDirection == true ? xFrames : yFrames);
	t.subImageSizeX = imgSize.x / xFrames;
	t.subImageSizeY = imgSize.y / yFrames;

	return true;
}

static void getTexture(const MultiTexture& t, size_t index,
	const sf::Texture** texture, sf::IntRect& textureRect) noexcept
{
	if (t.horizontalDirection == true)
	{
		textureRect.left = (int)((index % t.maxFrames) * t.subImageSizeX);
		textureRect.top = (int)((index / t.maxFrames) * t.subImageSizeY);
	}
	else
	{
		textureRect.left = (int)((index / t.maxFrames) * t.subImageSizeX);
		textureRect.top = (int)((index % t.maxFrames) * t.subImageSizeY);
	}
	textureRect.width = t.subImageSizeX;
	textureRect.height = t.subImageSizeY;
	(*texture) = t.texture.get();
}

SimpleTexturePack::SimpleTexturePack(const std::shared_ptr<sf::Texture>& texture_,
	size_t xFrames_, size_t yFrames_, bool horizontalDirection_,
	const std::shared_ptr<Palette>& palette_, bool isIndexed_)
	: palette(palette_), indexed(isIndexed_)
{
	ConstructTexturePack(t, texture_, xFrames_, yFrames_, horizontalDirection_);
}

bool SimpleTexturePack::get(size_t index,
	const sf::Texture** texture, sf::IntRect& textureRect) const noexcept
{
	if (t.numFrames == 0 ||
		index >= t.numFrames)
	{
		return false;
	}
	getTexture(t, index, texture, textureRect);
	return true;
}

bool SimpleTexturePack::get(size_t indexX, size_t indexY,
	const sf::Texture** texture, sf::IntRect& textureRect) const noexcept
{
	return get(indexX, texture, textureRect);
}

bool SimpleTexturePack::getTextureSize(sf::Vector2i& textureSize) const noexcept
{
	textureSize.x = (int)t.subImageSizeX;
	textureSize.y = (int)t.subImageSizeY;
	return true;
}

bool SimpleMultiTexturePack::get(size_t index,
	const sf::Texture** texture, sf::IntRect& textureRect) const
{
	if (texVec.empty() == true ||
		index >= textureCount)
	{
		return false;
	}
	if (texturesHaveSameNumFrames() == true)
	{
		size_t indexX = index % numFrames;
		size_t indexY = index / numFrames;
		getTexture(texVec[indexY], indexX, texture, textureRect);
		return true;
	}
	size_t indexX = index;
	size_t indexY = 0;
	while (indexX >= texVec[indexY].numFrames)
	{
		indexX -= texVec[indexY].numFrames;
		indexY++;
		if (indexY >= texVec.size())
		{
			return false;
		}
	}
	getTexture(texVec[indexY], indexX, texture, textureRect);
	return true;
}

bool SimpleMultiTexturePack::get(size_t indexX, size_t indexY,
	const sf::Texture** texture, sf::IntRect& textureRect) const
{
	if (indexY >= texVec.size() ||
		indexX >= texVec[indexY].numFrames)
	{
		return false;
	}
	getTexture(texVec[indexY], indexX, texture, textureRect);
	return true;
}

bool SimpleMultiTexturePack::getTextureSize(sf::Vector2i& textureSize) const
{
	if (texturesHaveSameSize == false)
	{
		return false;
	}
	textureSize.x = (int)texVec.front().subImageSizeX;
	textureSize.y = (int)texVec.front().subImageSizeY;
	return true;
}

void SimpleMultiTexturePack::addTexturePack(const std::shared_ptr<sf::Texture>& texture,
	size_t xFrames, size_t yFrames, bool horizontalDirection)
{
	MultiTexture t;
	if (ConstructTexturePack(t, texture, xFrames, yFrames, horizontalDirection) == true)
	{
		if (texVec.empty() == true)
		{
			texturesHaveSameSize = true;
			numFrames = t.numFrames;
		}
		else
		{
			if (texturesHaveSameSize == true && (
				t.subImageSizeX != texVec.back().subImageSizeX ||
				t.subImageSizeY != texVec.back().subImageSizeY))
			{
				texturesHaveSameSize = false;
			}
			if (texturesHaveSameNumFrames() == true &&
				numFrames != t.numFrames)
			{
				numFrames = 0;
			}
		}
		texVec.push_back(t);
		textureCount += t.numFrames;
	}
}
