#include "SimpleTexturePack.h"
#include <limits>
#include "TextureInfo.h"

static bool ConstructTexturePack(MultiTexture& t, const std::shared_ptr<sf::Texture>& texture,
	const std::pair<uint32_t, uint32_t>& frames, const sf::Vector2f& offset,
	uint32_t startIndex, uint32_t directions, bool horizontalDirection,
	AnimationType animType)
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

	if (directions > 0 && (t.numFrames % directions) == 0)
	{
		t.directions = directions;
	}
	t.animType = animType;

	return true;
}

static void getTexture(const MultiTexture& t, uint32_t index, TextureInfo& ti) noexcept
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
	ti.blendMode = BlendMode::Alpha;
	ti.nextIndex = -1;
}

static uint32_t getDirectionHelper(const MultiTexture& t, uint32_t frameIdx) noexcept
{
	if (frameIdx >= t.startIndex && frameIdx < t.startIndex + t.numFrames)
	{
		if (t.directions <= 1)
		{
			return 0;
		}
		auto framesPerDirection = t.numFrames / t.directions;
		return (frameIdx - t.startIndex) / framesPerDirection;
	}
	return std::numeric_limits<uint32_t>::max();
}

SimpleTexturePack::SimpleTexturePack(const std::shared_ptr<sf::Texture>& texture,
	const std::pair<uint32_t, uint32_t>& frames, const sf::Vector2f& offset,
	uint32_t startIndex, uint32_t directions_, bool horizontalDirection,
	AnimationType animType, const std::shared_ptr<Palette>& palette_)
	: palette(palette_)
{
	ConstructTexturePack(
		t, texture, frames, offset, startIndex,
		directions_, horizontalDirection, animType
	);
}

bool SimpleTexturePack::get(uint32_t index, TextureInfo& ti) const noexcept
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
	::getTexture(t, index, ti);
	ti.palette = palette;
	return true;
}

uint32_t SimpleTexturePack::getDirection(uint32_t frameIdx) const noexcept
{
	auto direction = getDirectionHelper(t, frameIdx);
	if (direction != std::numeric_limits<uint32_t>::max())
	{
		return direction;
	}
	return 0;
}

std::pair<uint32_t, uint32_t> SimpleTexturePack::getRange(
	int32_t groupIdx, int32_t directionIdx, AnimationType& animType) const
{
	animType = t.animType;
	if (directionIdx >= 0 && (uint32_t)directionIdx < t.directions)
	{
		return TexturePack::getRange(
			t.startIndex,
			t.startIndex + t.numFrames,
			directionIdx,
			t.directions
		);
	}
	return std::make_pair(t.startIndex, t.startIndex + t.numFrames - 1);
}

bool SimpleMultiTexturePack::get(uint32_t index, TextureInfo& ti) const
{
	if (texVec.empty() == true)
	{
		return false;
	}
	if (indexesHaveGaps == false &&
		texturesHaveSameNumFrames() == true)
	{
		uint32_t indexX = index % numFrames;
		uint32_t indexY = index / numFrames;
		if (indexY >= texVec.size() ||
			indexX >= texVec[indexY].numFrames)
		{
			return false;
		}
		::getTexture(texVec[indexY], indexX, ti);
		ti.palette = palette;
		return true;
	}
	uint32_t indexX;
	uint32_t indexY = std::numeric_limits<uint32_t>::max();
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

	::getTexture(texVec[indexY], indexX, ti);
	ti.palette = palette;
	return true;
}

void SimpleMultiTexturePack::addTexturePack(const std::shared_ptr<sf::Texture>& texture,
	const std::pair<uint32_t, uint32_t>& frames, const sf::Vector2f& offset,
	uint32_t startIndex, uint32_t directions, bool horizontalDirection,
	AnimationType animType)
{
	if (startIndex < textureCount)
	{
		startIndex = textureCount;
	}
	MultiTexture t;
	auto success = ConstructTexturePack(
		t, texture, frames, offset, startIndex,
		directions, horizontalDirection, animType
	);
	if (success == true)
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

const sf::Texture* SimpleMultiTexturePack::getTexture() const noexcept
{
	if (texVec.size() == 1)
	{
		return texVec.front().texture.get();
	}
	return nullptr;
}

uint32_t SimpleMultiTexturePack::getDirectionCount(uint32_t groupIdx) const noexcept
{
	if (groupIdx < texVec.size())
	{
		return texVec[groupIdx].directions;
	}
	return 1;
}

uint32_t SimpleMultiTexturePack::getDirection(uint32_t frameIdx) const noexcept
{
	for (const auto& t : texVec)
	{
		auto direction = getDirectionHelper(t, frameIdx);
		if (direction != std::numeric_limits<uint32_t>::max())
		{
			return direction;
		}
	}
	return 0;
}

std::pair<uint32_t, uint32_t> SimpleMultiTexturePack::getRange(
	int32_t groupIdx, int32_t directionIdx, AnimationType& animType) const
{
	if (groupIdx >= 0 && (uint32_t)groupIdx < texVec.size())
	{
		animType = texVec[groupIdx].animType;

		return TexturePack::getRange(
			texVec[groupIdx].startIndex,
			texVec[groupIdx].startIndex + texVec[groupIdx].numFrames,
			directionIdx,
			texVec[groupIdx].directions
		);
	}
	animType = {};
	return std::make_pair((uint32_t)0, textureCount - 1);
}
