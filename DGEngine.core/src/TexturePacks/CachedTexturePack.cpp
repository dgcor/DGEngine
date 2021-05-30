#include "CachedTexturePack.h"
#include "AnimationInfo.h"

CachedTexturePack::CachedTexturePack(const std::shared_ptr<ImageContainer>& imgPack_,
	const sf::Vector2f& offset_, const std::shared_ptr<Palette>& palette_, bool isIndexed_)
	: imgPack(imgPack_), offset(offset_), palette(palette_), indexed(isIndexed_)
{
	cache.resize(imgPack_->size());
}

bool CachedTexturePack::fetchTexture(uint32_t index) const
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
	return true;
}

bool CachedTexturePack::get(uint32_t index, TextureInfo& ti) const
{
	if (fetchTexture(index) == false)
	{
		return false;
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

int32_t CachedTexturePack::getWidth(uint32_t index) const
{
	if (fetchTexture(index) == false)
	{
		return 0;
	}
	return (int32_t)getTextureWidth(cache[index].first);
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
		return frameIdx / framesPerDirection;
	}
	return 0;
}

AnimationInfo CachedTexturePack::getAnimation(int32_t groupIdx, int32_t directionIdx) const
{
	AnimationInfo animInfo;
	animInfo.indexRange = TexturePack::getRange(
		0,
		(uint32_t)cache.size(),
		directionIdx,
		imgPack->getDirections()
	);
	return animInfo;
}

CachedMultiTexturePack::CachedMultiTexturePack(
	const std::vector<std::shared_ptr<ImageContainer>>& imgVec_,
	const sf::Vector2f& offset_, const std::shared_ptr<Palette>& palette_, bool isIndexed_)
	: imgVec(imgVec_), offset(offset_), palette(palette_), indexed(isIndexed_)
{
	for (const auto& imgPack : imgVec_)
	{
		textureCount += imgPack->size();
	}
	cache.resize(textureCount);
}

bool CachedMultiTexturePack::fetchTexture(uint32_t index) const
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
	return true;
}

bool CachedMultiTexturePack::get(uint32_t index, TextureInfo& ti) const
{
	if (fetchTexture(index) == false)
	{
		return false;
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

int32_t CachedMultiTexturePack::getWidth(uint32_t index) const
{
	if (fetchTexture(index) == false)
	{
		return 0;
	}
	return (int32_t)getTextureWidth(cache[index].first);
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
			return (frameIdx - startIdx) / framesPerDirection;
		}
		startIdx += numFrames;
	}
	return 0;
}

AnimationInfo CachedMultiTexturePack::getAnimation(int32_t groupIdx, int32_t directionIdx) const
{
	AnimationInfo animInfo;
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
				animInfo.indexRange = TexturePack::getRange(
					startIdx,
					stopIdx,
					directionIdx,
					directions
				);
				return animInfo;
			}
			startIdx = stopIdx;
		}
	}
	animInfo.indexRange = std::make_pair((uint32_t)0, textureCount - 1);
	return animInfo;
}
