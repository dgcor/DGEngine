#include "SimpleTexturePack.h"
#include "AnimationInfo.h"
#include <limits>

static bool makeTexturePack(MultiTexture& t, const std::pair<uint32_t, uint32_t>& frames)
{
	if (t.texture == nullptr)
	{
		return false;
	}
	t.numFrames = frames.first * frames.second;
	if (t.numFrames == 0)
	{
		return false;
	}

	auto imgSize = t.texture->getSize();

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

	t.maxFrames = (t.horizontalDirection == true ? frames.first : frames.second);
	t.subImageSizeX = imgSize.x / frames.first;
	t.subImageSizeY = imgSize.y / frames.second;

	if (t.directionsVec.empty() == true)
	{
		if (t.directions == 0 || (t.numFrames % t.directions) != 0)
		{
			t.directions = 1;
		}
	}
	else
	{
		t.directions = (uint32_t)t.directionsVec.size();
	}
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
		if (t.directionsVec.empty() == false)
		{
			for (uint32_t i = 0; i < t.directionsVec.size(); i++)
			{
				const auto& range = t.directionsVec[i];
				auto idx = frameIdx - t.startIndex;
				if (idx >= range.first && idx < range.second)
				{
					return i;
				}
			}
			return 0;
		}
		else
		{
			auto framesPerDirection = t.numFrames / t.directions;
			return (frameIdx - t.startIndex) / framesPerDirection;
		}
	}
	return std::numeric_limits<uint32_t>::max();
}

SingleTexturePack::SingleTexturePack(MultiTexture&& t_,
	const std::shared_ptr<Palette>& palette_) : t(std::move(t_)), palette(palette_) {}

SingleTexturePack::SingleTexturePack(MultiTexture&& t_,
	const std::pair<uint32_t, uint32_t>& frames, const std::shared_ptr<Palette>& palette_)
	: t(std::move(t_)), palette(palette_)
{
	makeTexturePack(t, frames);
}

bool SingleTexturePack::fetchIndex(uint32_t& index) const
{
	if (t.numFrames == 0 || index < t.startIndex)
	{
		return false;
	}
	index -= t.startIndex;
	if (index >= t.numFrames)
	{
		return false;
	}
	return true;
}

bool SingleTexturePack::get(uint32_t index, TextureInfo& ti) const noexcept
{
	if (fetchIndex(index) == false)
	{
		return false;
	}
	::getTexture(t, index, ti);
	ti.palette = palette;
	return true;
}

int32_t SingleTexturePack::getWidth(uint32_t index) const
{
	if (fetchIndex(index) == false)
	{
		return 0;
	}
	return (int32_t)t.subImageSizeX;
}

uint32_t SingleTexturePack::getDirection(uint32_t frameIdx) const noexcept
{
	auto direction = getDirectionHelper(t, frameIdx);
	if (direction != std::numeric_limits<uint32_t>::max())
	{
		return direction;
	}
	return 0;
}

AnimationInfo SingleTexturePack::getAnimation(int32_t groupIdx, int32_t directionIdx) const
{
	AnimationInfo animInfo;
	animInfo.animType = t.animType;
	animInfo.refresh = t.refresh;
	if (directionIdx >= 0)
	{
		if (t.directionsVec.empty() == false)
		{
			if ((uint32_t)directionIdx < t.directionsVec.size())
			{
				animInfo.indexRange = t.directionsVec[directionIdx];
				return animInfo;
			}
		}
		else if ((uint32_t)directionIdx < t.directions)
		{
			animInfo.indexRange = TexturePack::getRange(
				t.startIndex,
				t.startIndex + t.numFrames,
				directionIdx,
				t.directions
			);
			return animInfo;
		}
	}
	animInfo.indexRange = std::make_pair(t.startIndex, t.startIndex + t.numFrames - 1);
	return animInfo;
}

SimpleTexturePack::SimpleTexturePack(MultiTexture&& t_,
	const std::shared_ptr<Palette>& palette_) : SingleTexturePack(std::move(t_), palette_)
{
	if (t.texture == nullptr)
	{
		return;
	}
	t.numFrames = 1;
	t.maxFrames = 1;
	auto imgSize = t.texture->getSize();
	t.subImageSizeX = imgSize.x;
	t.subImageSizeY = imgSize.y;
	if (t.directionsVec.empty() == false)
	{
		t.directions = (uint32_t)t.directionsVec.size();
	}
	else
	{
		t.directions = std::max(t.directions, 1u);
	}
	palette = palette_;
}

bool SimpleTexturePack::get(uint32_t index, TextureInfo& ti) const noexcept
{
	if (index > 0)
	{
		return false;
	}
	ti.texture = t.texture.get();
	ti.textureRect.top = 0;
	ti.textureRect.left = 0;
	ti.textureRect.width = t.subImageSizeX;
	ti.textureRect.height = t.subImageSizeY;
	ti.palette = palette;
	ti.offset = t.offset;
	ti.absoluteOffset = false;
	ti.blendMode = BlendMode::Alpha;
	ti.nextIndex = -1;
	return true;
}

bool MultiTexturePack::fetchIndex(uint32_t index,
	uint32_t& indexX, uint32_t& indexY) const
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
	::getTexture(texVec[indexY], indexX, ti);
	ti.palette = palette;
	return true;
}

int32_t MultiTexturePack::getWidth(uint32_t index) const
{
	uint32_t indexX, indexY;
	if (fetchIndex(index, indexX, indexY) == false)
	{
		return 0;
	}
	return (int32_t)texVec[indexY].subImageSizeX;
}

void MultiTexturePack::addTexturePack(MultiTexture&& t,
	const std::pair<uint32_t, uint32_t>& frames)
{
	if (t.startIndex < textureCount)
	{
		t.startIndex = textureCount;
	}
	if (makeTexturePack(t, frames) == true)
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
		auto direction = getDirectionHelper(t, frameIdx);
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
