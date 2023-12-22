#include "ImageContainerTexturePack.h"
#include "TexturePackUtils.h"

ImageContainerTexturePack::ImageContainerTexturePack(
	const std::shared_ptr<ImageContainer>& imgPack_, const sf::Vector2f& offset_,
	const std::shared_ptr<Palette>& palette_, bool isIndexed_) : imgVec(1, imgPack_),
	offset(offset_), palette(palette_), indexed(isIndexed_)
{
	textureCount = imgPack_->size();
	cache.resize(textureCount);
}

ImageContainerTexturePack::ImageContainerTexturePack(
	const std::vector<std::shared_ptr<ImageContainer>>& imgVec_,
	const sf::Vector2f& offset_, const std::shared_ptr<Palette>& palette_, bool isIndexed_)
	: imgVec(imgVec_.begin(), imgVec_.end()), offset(offset_), palette(palette_), indexed(isIndexed_)
{
	for (const auto& imgPack : imgVec_)
	{
		textureCount += imgPack->size();
	}
	cache.resize(textureCount);
}

bool ImageContainerTexturePack::fetchTexture(uint32_t index) const
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

bool ImageContainerTexturePack::get(uint32_t index, TextureInfo& ti) const
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

sf::Vector2i ImageContainerTexturePack::getTextureSize(uint32_t index) const
{
	if (fetchTexture(index) == false)
	{
		return {};
	}
	auto size = cache[index].first.getSize();
	return { (int32_t)size.x, (int32_t)size.y };
}

uint32_t ImageContainerTexturePack::getDirectionCount(uint32_t groupIdx) const noexcept
{
	if (groupIdx < imgVec.size())
	{
		return imgVec[groupIdx]->getDirections();
	}
	return 1;
}

std::pair<uint32_t, uint32_t> ImageContainerTexturePack::getDirection(uint32_t frameIdx, AnimationFlags& flags) const noexcept
{
	for (uint32_t groupIdx = 0, startIdx = 0; const auto& imgPack : imgVec)
	{
		auto numFrames = imgPack->size();
		if (frameIdx < startIdx + numFrames)
		{
			auto directions = imgPack->getDirections();
			return { groupIdx, TexturePackUtils::calculateDirection(
				frameIdx,
				startIdx,
				numFrames,
				directions,
				flags
			) };
		}
		groupIdx++;
		startIdx += numFrames;
	}
	flags = AnimationFlags::Overflow;
	return {};
}

AnimationInfo ImageContainerTexturePack::getAnimation(int32_t groupIdx, int32_t directionIdx) const
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
				TexturePackUtils::calculateRange(
					startIdx,
					stopIdx > 0 ? stopIdx - 1 : 0,
					directionIdx,
					directions,
					animInfo
				);
				return animInfo;
			}
			startIdx = stopIdx;
		}
	}
	animInfo.indexRange = { (uint32_t)0, textureCount - 1 };
	animInfo.flags = AnimationFlags::Overflow;
	return animInfo;
}
