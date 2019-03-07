#include "SimpleTexturePack.h"

static bool ConstructTexturePack(MultiTexture& t, const std::shared_ptr<sf::Texture>& texture,
	const std::pair<size_t, size_t>& frames, const sf::Vector2f& offset,
	size_t startIndex, bool horizontalDirection)
{
	if (texture == nullptr)
	{
		return false;
	}
	t.texture = texture;
	t.offset = offset;
	t.startIndex = startIndex;
	t.horizontalDirection = horizontalDirection;
	t.numFrames = frames.first * frames.second;
	if (t.numFrames == 0)
	{
		return false;
	}

	auto imgSize = texture->getSize();

	if (imgSize.x == 0 ||
		imgSize.y == 0 ||
		frames.first > imgSize.x ||
		frames.second > imgSize.y ||
		imgSize.x % frames.first != 0 ||
		imgSize.y % frames.second != 0)
	{
		t.numFrames = 0;
		return false;
	}

	t.maxFrames = (horizontalDirection == true ? frames.first : frames.second);
	t.subImageSizeX = imgSize.x / frames.first;
	t.subImageSizeY = imgSize.y / frames.second;

	return true;
}

static void getTexture(const MultiTexture& t, size_t index, TextureInfo& ti) noexcept
{
	if (t.numFrames <= 1 && index == 0)
	{
		ti.textureRect.left = 0;
		ti.textureRect.top = 0;
	}
	else if (t.horizontalDirection == true)
	{
		ti.textureRect.left = (int)((index % t.maxFrames) * t.subImageSizeX);
		ti.textureRect.top = (int)((index / t.maxFrames) * t.subImageSizeY);
	}
	else
	{
		ti.textureRect.left = (int)((index / t.maxFrames) * t.subImageSizeX);
		ti.textureRect.top = (int)((index % t.maxFrames) * t.subImageSizeY);
	}
	ti.textureRect.width = t.subImageSizeX;
	ti.textureRect.height = t.subImageSizeY;
	ti.texture = t.texture.get();
	ti.offset = t.offset;
	ti.absoluteOffset = false;
}

SimpleTexturePack::SimpleTexturePack(const std::shared_ptr<sf::Texture>& texture,
	const std::pair<size_t, size_t>& frames, const sf::Vector2f& offset,
	size_t startIndex, bool horizontalDirection, const std::shared_ptr<Palette>& palette_,
	bool isIndexed) : palette(palette_), indexed(isIndexed)
{
	ConstructTexturePack(t, texture, frames, offset, startIndex, horizontalDirection);
}

bool SimpleTexturePack::get(size_t index, TextureInfo& ti) const noexcept
{
	if (t.numFrames == 0 ||
		index < t.startIndex)
	{
		return false;
	}
	index -= t.startIndex;
	if (index >= t.numFrames)
	{
		return false;
	}
	getTexture(t, index, ti);
	ti.palette = palette;
	return true;
}

bool SimpleMultiTexturePack::get(size_t index, TextureInfo& ti) const
{
	if (texVec.empty() == true)
	{
		return false;
	}
	if (indexesHaveGaps == false &&
		texturesHaveSameNumFrames() == true)
	{
		size_t indexX = index % numFrames;
		size_t indexY = index / numFrames;
		if (indexY >= texVec.size() ||
			indexX >= texVec[indexY].numFrames)
		{
			return false;
		}
		getTexture(texVec[indexY], indexX, ti);
		ti.palette = palette;
		return true;
	}
	size_t indexX;
	size_t indexY = std::numeric_limits<size_t>::max();
	do
	{
		indexY++;
		if (indexY >= texVec.size() ||
			index < texVec[indexY].startIndex)
		{
			return false;
		}
		indexX = index - texVec[indexY].startIndex;

	} while (indexX >= texVec[indexY].numFrames);

	getTexture(texVec[indexY], indexX, ti);
	ti.palette = palette;
	return true;
}

void SimpleMultiTexturePack::addTexturePack(const std::shared_ptr<sf::Texture>& texture,
	const std::pair<size_t, size_t>& frames, const sf::Vector2f& offset,
	size_t startIndex, bool horizontalDirection)
{
	if (startIndex < textureCount)
	{
		startIndex = textureCount;
	}
	MultiTexture t;
	if (ConstructTexturePack(t, texture, frames, offset, startIndex, horizontalDirection) == true)
	{
		if (texVec.empty() == true)
		{
			texturesHaveSameSize = true;
			numFrames = t.numFrames;
			if (t.startIndex > 0)
			{
				indexesHaveGaps = true;
			}
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
			if (indexesHaveGaps == false &&
				(t.startIndex > (texVec.back().startIndex + texVec.back().numFrames)))
			{
				indexesHaveGaps = true;
			}
		}
		texVec.push_back(t);
		textureCount += t.numFrames;
	}
}
