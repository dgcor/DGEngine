#include "MultiTexturePack.h"
#include "Game/AnimationInfo.h"
#include <limits>

MultiTexturePack::MultiTexturePack(TextureGroup&& textureGroup_,
	const std::shared_ptr<Palette>& palette_) : palette(palette_)
{
	addTextureGroup(std::move(textureGroup_));
}

bool MultiTexturePack::fetchIndex(uint32_t index, uint32_t& indexX, uint32_t& indexY) const
{
	if (textureGroups.empty() == true)
	{
		return false;
	}
	if (indexesHaveGaps == false && texturesHaveSameNumFrames() == true)
	{
		indexX = index % numFrames;
		indexY = index / numFrames;
		if (indexY >= textureGroups.size() || indexX >= textureGroups[indexY].numFrames)
		{
			return false;
		}
		return true;
	}
	indexY = std::numeric_limits<uint32_t>::max();
	do
	{
		indexY++;
		if (indexY >= textureGroups.size() || index < textureGroups[indexY].startIndex)
		{
			return false;
		}
		indexX = index - textureGroups[indexY].startIndex;

	} while (indexX >= textureGroups[indexY].numFrames);
	return true;
}

bool MultiTexturePack::get(uint32_t index, TextureInfo& ti) const
{
	uint32_t indexX, indexY;
	if (fetchIndex(index, indexX, indexY) == false)
	{
		return false;
	}
	textureGroups[indexY].getTexture(indexX, ti);
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
	return { (int32_t)textureGroups[indexY].subImageSizeX, (int32_t)textureGroups[indexY].subImageSizeY };
}

void MultiTexturePack::addTextureGroup(TextureGroup&& textureGroup)
{
	if (textureGroup.startIndex < textureCount)
	{
		textureGroup.startIndex = textureCount;
	}
	if (textureGroup.isValid() == true)
	{
		if (textureGroups.empty() == true)
		{
			texturesHaveSameSize = true;
			numFrames = textureGroup.numFrames;
			if (textureGroup.startIndex > 0)
			{
				indexesHaveGaps = true;
			}
			frameRange.first = textureGroup.startIndex;
			frameRange.second = textureGroup.startIndex + numFrames - 1;
		}
		else
		{
			if (texturesHaveSameSize == true && (
				textureGroup.subImageSizeX != textureGroups.back().subImageSizeX ||
				textureGroup.subImageSizeY != textureGroups.back().subImageSizeY))
			{
				texturesHaveSameSize = false;
			}
			if (texturesHaveSameNumFrames() == true &&
				numFrames != textureGroup.numFrames)
			{
				numFrames = 0;
			}
			auto frameRangeEnd = textureGroups.back().startIndex + textureGroups.back().numFrames;
			if (textureGroup.startIndex < frameRangeEnd)
			{
				textureGroup.startIndex = frameRangeEnd;
			}
			auto indexGap = textureGroup.startIndex > frameRangeEnd ? textureGroup.startIndex - frameRangeEnd : 0u;
			if (indexesHaveGaps == false && indexGap > 0)
			{
				indexesHaveGaps = true;
			}
			frameRange.second += textureGroups.back().numFrames + indexGap;
		}
		textureCount += textureGroup.numFrames;
		textureGroups.push_back(std::move(textureGroup));
	}
}

const sf::Texture* MultiTexturePack::getTexture() const noexcept
{
	if (textureGroups.size() == 1)
	{
		return textureGroups.front().texture.get();
	}
	return nullptr;
}

uint32_t MultiTexturePack::getGroupCount() const noexcept
{
	return textureGroups.empty() == false ? (uint32_t)textureGroups.size() : 1;
}

std::pair<uint32_t, uint32_t> MultiTexturePack::getDirection(uint32_t frameIdx, AnimationFlags& flags) const noexcept
{
	for (uint32_t groupIdx = 0; const auto& textureGroup : textureGroups)
	{
		auto direction = textureGroup.getDirection(frameIdx, flags);
		if ((int)(flags & AnimationFlags::Valid) != 0)
		{
			return { groupIdx, direction };
		}
		groupIdx++;
	}
	flags = AnimationFlags::Overflow;
	return {};
}

AnimationInfo MultiTexturePack::getAnimation(int32_t groupIdx, int32_t directionIdx) const
{
	if (textureGroups.size() == 1)
	{
		return textureGroups.front().getAnimation(groupIdx, directionIdx);
	}
	if (groupIdx >= 0 && (uint32_t)groupIdx < textureGroups.size())
	{
		return textureGroups[groupIdx].getAnimation(0, directionIdx);
	}
	AnimationInfo animInfo;
	animInfo.indexRange = frameRange;
	return animInfo;
}
