#include "CachedTexturePack.h"
#include "TextureInfo.h"

static uint32_t getNormalizedDirection(uint32_t direction, uint32_t numberOfDirections)
{
	if (numberOfDirections == 8 ||
		numberOfDirections == 16)
	{
		switch (direction)
		{
		case 0:
			return 4;
		case 1:
			return 0;
		case 2:
			return 5;
		case 3:
			return 1;
		case 4:
			return 6;
		case 5:
			return 2;
		case 6:
			return 7;
		case 7:
			return 3;
		default:
			return direction;
		}
	}
	return direction;
}

CachedTexturePack::CachedTexturePack(const std::shared_ptr<ImageContainer>& imgPack_,
	const sf::Vector2f& offset_, const std::shared_ptr<Palette>& palette_,
	bool isIndexed_, bool normalizeDirections_) : imgPack(imgPack_), offset(offset_),
	palette(palette_), indexed(isIndexed_), normalizeDirections(normalizeDirections_)
{
	cache.resize(imgPack_->size());
}

bool CachedTexturePack::get(uint32_t index, TextureInfo& ti) const
{
	if (index >= imgPack->size())
	{
		return false;
	}
	if (cache[index].first.getNativeHandle() == 0)
	{
		PaletteArray* palArray = nullptr;
		if (indexed == false && palette != nullptr)
		{
			palArray = &palette->palette;
		}
		cache[index].first = imgPack->get(
			index,
			palArray,
			cache[index].second
		);
	}
	ti.texture = &cache[index].first;
	updateTextureRect(ti);
	ti.palette = palette;
	ti.offset = cache[index].second.offset + offset;
	ti.absoluteOffset = cache[index].second.absoluteOffset;
	ti.blendMode = cache[index].second.blendMode;
	ti.nextIndex = cache[index].second.nextIndex;
	return true;
}

uint32_t CachedTexturePack::getDirectionCount(uint32_t groupIdx) const noexcept
{
	return imgPack->getDirections();
}

uint32_t CachedTexturePack::getDirection(uint32_t frameIdx) const noexcept
{
	auto numFrames = imgPack->size();
	if (frameIdx < numFrames)
	{
		auto directions = imgPack->getDirections();
		if (directions <= 1)
		{
			return 0;
		}
		auto framesPerDirection = numFrames / directions;
		if (normalizeDirections == false)
		{
			return frameIdx / framesPerDirection;
		}
		else
		{
			return getNormalizedDirection(
				frameIdx / framesPerDirection,
				directions
			);
		}
	}
	return 0;
}

std::pair<uint32_t, uint32_t> CachedTexturePack::getRange(
	int32_t groupIdx, int32_t directionIdx, AnimationType& animType) const
{
	animType = {};
	auto directions = imgPack->getDirections();
	if (normalizeDirections == true && directions > 1)
	{
		directionIdx = (int32_t)getNormalizedDirection(directionIdx, directions);
	}
	return TexturePack::getRange(
		0,
		cache.size(),
		directionIdx,
		directions
	);
}

CachedMultiTexturePack::CachedMultiTexturePack(
	const std::vector<std::shared_ptr<ImageContainer>>& imgVec_,
	const sf::Vector2f& offset_, const std::shared_ptr<Palette>& palette_,
	bool isIndexed_, bool normalizeDirections_) : imgVec(imgVec_), offset(offset_),
	palette(palette_), indexed(isIndexed_), normalizeDirections(normalizeDirections_)
{
	for (const auto& imgPack : imgVec_)
	{
		textureCount += imgPack->size();
	}
	cache.resize(textureCount);
}

bool CachedMultiTexturePack::get(uint32_t index, TextureInfo& ti) const
{
	if (imgVec.empty() == true ||
		index >= textureCount)
	{
		return false;
	}
	if (cache[index].first.getNativeHandle() == 0)
	{
		uint32_t indexX = index;
		uint32_t indexY = 0;
		while (indexX >= imgVec[indexY]->size())
		{
			indexX -= imgVec[indexY]->size();
			indexY++;
			if (indexY >= imgVec.size())
			{
				return false;
			}
		}
		PaletteArray* palArray = nullptr;
		if (indexed == false && palette != nullptr)
		{
			palArray = &palette->palette;
		}
		cache[index].first = imgVec[indexY]->get(
			indexX,
			palArray,
			cache[index].second
		);
	}
	ti.texture = &cache[index].first;
	updateTextureRect(ti);
	ti.palette = palette;
	ti.offset = cache[index].second.offset + offset;
	ti.absoluteOffset = cache[index].second.absoluteOffset;
	ti.blendMode = cache[index].second.blendMode;
	ti.nextIndex = -1;
	return true;
}

uint32_t CachedMultiTexturePack::getDirectionCount(uint32_t groupIdx) const noexcept
{
	if (groupIdx < imgVec.size())
	{
		return imgVec[groupIdx]->getDirections();
	}
	return 1;
}

uint32_t CachedMultiTexturePack::getDirection(uint32_t frameIdx) const noexcept
{
	uint32_t startIdx = 0;
	for (const auto& imgPack : imgVec)
	{
		auto numFrames = imgPack->size();
		if (frameIdx < numFrames)
		{
			auto directions = imgPack->getDirections();
			if (directions <= 1)
			{
				return 0;
			}
			auto framesPerDirection = numFrames / directions;
			if (normalizeDirections == false)
			{
				return (frameIdx - startIdx) / framesPerDirection;
			}
			else
			{
				return getNormalizedDirection(
					(frameIdx - startIdx) / framesPerDirection,
					directions
				);
			}
		}
		startIdx += numFrames;
	}
	return 0;
}

std::pair<uint32_t, uint32_t> CachedMultiTexturePack::getRange(
	int32_t groupIdx, int32_t directionIdx, AnimationType& animType) const
{
	animType = {};
	if (groupIdx >= 0 && (uint32_t)groupIdx < imgVec.size())
	{
		uint32_t startIdx = 0;
		uint32_t stopIdx = 0;
		for (size_t i = 0; i < imgVec.size(); i++)
		{
			stopIdx += imgVec[i]->size();
			if (i == (size_t)groupIdx)
			{
				auto directions = imgVec[i]->getDirections();
				if (normalizeDirections == true && directions > 1)
				{
					directionIdx = (int32_t)getNormalizedDirection(directionIdx, directions);
				}
				return TexturePack::getRange(
					startIdx,
					stopIdx,
					directionIdx,
					directions
				);
			}
			startIdx = stopIdx;
		}
	}
	return std::make_pair((uint32_t)0, textureCount - 1);
}
