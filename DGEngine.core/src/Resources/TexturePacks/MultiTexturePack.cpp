#include "MultiTexturePack.h"
#include "Game/AnimationInfo.h"
#include <limits>

bool MultiTexturePack::fetchIndex(uint32_t index, uint32_t& indexX, uint32_t& indexY) const
{
	if (texVec.empty() == true)
	{
		return false;
	}
	if (indexesHaveGaps == false && texturesHaveSameNumFrames() == true)
	{
		indexX = index % numFrames;
		indexY = index / numFrames;
		if (indexY >= texVec.size() || indexX >= texVec[indexY].numFrames)
		{
			return false;
		}
		return true;
	}
	indexY = std::numeric_limits<uint32_t>::max();
	do
	{
		indexY++;
		if (indexY >= texVec.size() || index < texVec[indexY].startIndex)
		{
			return false;
		}
		indexX = index - texVec[indexY].startIndex;

	} while (indexX >= texVec[indexY].numFrames);
	return true;
}

bool MultiTexturePack::get(uint32_t index, TextureInfo& ti) const
{
	uint32_t indexX, indexY;
	if (fetchIndex(index, indexX, indexY) == false)
	{
		return false;
	}
	texVec[indexY].getTexture(indexX, ti);
	ti.palette = palette;
	return true;
}

sf::Vector2i MultiTexturePack::getTextureSize(uint32_t index) const
{
	uint32_t indexX, indexY;
	if (fetchIndex(index, indexX, indexY) == false)
	{
		return {};
	}
	return { (int32_t)texVec[indexY].subImageSizeX, (int32_t)texVec[indexY].subImageSizeY };
}

void MultiTexturePack::addTexturePack(TexturePackGroup&& t, const std::pair<uint32_t, uint32_t>& frames)
{
	if (t.startIndex < textureCount)
	{
		t.startIndex = textureCount;
	}
	if (t.makeTexturePack(frames) == true)
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
		textureCount += t.numFrames;
		texVec.push_back(std::move(t));
	}
}

const sf::Texture* MultiTexturePack::getTexture() const noexcept
{
	if (texVec.size() == 1)
	{
		return texVec.front().texture.get();
	}
	return nullptr;
}

uint32_t MultiTexturePack::getDirectionCount(uint32_t groupIdx) const noexcept
{
	if (groupIdx < texVec.size())
	{
		return texVec[groupIdx].directions;
	}
	return 1;
}

uint32_t MultiTexturePack::getDirection(uint32_t frameIdx) const noexcept
{
	for (const auto& t : texVec)
	{
		auto direction = t.getDirection(frameIdx);
		if (direction != std::numeric_limits<uint32_t>::max())
		{
			return direction;
		}
	}
	return 0;
}

AnimationInfo MultiTexturePack::getAnimation(int32_t groupIdx, int32_t directionIdx) const
{
	AnimationInfo animInfo;
	if (groupIdx >= 0 && (uint32_t)groupIdx < texVec.size())
	{
		animInfo.indexRange = TexturePack::getRange(
			texVec[groupIdx].startIndex,
			texVec[groupIdx].startIndex + texVec[groupIdx].numFrames,
			directionIdx,
			texVec[groupIdx].directions
		);
		animInfo.animType = texVec[groupIdx].animType;
		animInfo.refresh = texVec[groupIdx].refresh;
	}
	else
	{
		animInfo.indexRange = std::make_pair((uint32_t)0, textureCount - 1);
	}
	return animInfo;
}
