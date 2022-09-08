#include "TexturePackGroup.h"
#include <limits>

bool TexturePackGroup::makeTexturePack(const std::pair<uint32_t, uint32_t>& frames)
{
	if (texture == nullptr)
	{
		return false;
	}
	numFrames = frames.first * frames.second;
	if (numFrames == 0)
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
		numFrames = 0;
		return false;
	}

	maxFrames = (horizontalDirection == true ? frames.first : frames.second);
	subImageSizeX = imgSize.x / frames.first;
	subImageSizeY = imgSize.y / frames.second;

	if (directionsVec.empty() == true)
	{
		if (directions == 0 || (numFrames % directions) != 0)
		{
			directions = 1;
		}
	}
	else
	{
		directions = (uint32_t)directionsVec.size();
	}
	return true;
}

void TexturePackGroup::getTexture(uint32_t index, TextureInfo& ti) const noexcept
{
	if (numFrames <= 1 && index == 0)
	{
		ti.textureRect.left = 0;
		ti.textureRect.top = 0;
	}
	else if (horizontalDirection == true)
	{
		ti.textureRect.left = (int)((index % maxFrames) * subImageSizeX);
		ti.textureRect.top = (int)((index / maxFrames) * subImageSizeY);
	}
	else
	{
		ti.textureRect.left = (int)((index / maxFrames) * subImageSizeX);
		ti.textureRect.top = (int)((index % maxFrames) * subImageSizeY);
	}
	ti.textureRect.width = subImageSizeX;
	ti.textureRect.height = subImageSizeY;
	ti.texture = texture.get();
	ti.offset = offset;
	ti.absoluteOffset = false;
	ti.blendMode = BlendMode::Alpha;
	ti.nextIndex = -1;
}

uint32_t TexturePackGroup::getDirection(uint32_t frameIdx) const noexcept
{
	if (frameIdx >= startIndex && frameIdx < startIndex + numFrames)
	{
		if (directions <= 1)
		{
			return 0;
		}
		if (directionsVec.empty() == false)
		{
			for (uint32_t i = 0; i < directionsVec.size(); i++)
			{
				const auto& range = directionsVec[i];
				auto idx = frameIdx - startIndex;
				if (idx >= range.first && idx < range.second)
				{
					return i;
				}
			}
			return 0;
		}
		else
		{
			auto framesPerDirection = numFrames / directions;
			return (frameIdx - startIndex) / framesPerDirection;
		}
	}
	return std::numeric_limits<uint32_t>::max();
}
