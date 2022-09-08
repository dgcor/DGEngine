#include "ImageContainerTexturePack.h"
#include "Game/AnimationInfo.h"

ImageContainerTexturePack::ImageContainerTexturePack(const std::shared_ptr<ImageContainer>& imgPack_,
	const sf::Vector2f& offset_, const std::shared_ptr<Palette>& palette_, bool isIndexed_)
	: imgPack(imgPack_), offset(offset_), palette(palette_), indexed(isIndexed_)
{
	cache.resize(imgPack_->size());
}

bool ImageContainerTexturePack::fetchTexture(uint32_t index) const
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
	return imgPack->getDirections();
}

uint32_t ImageContainerTexturePack::getDirection(uint32_t frameIdx) const noexcept
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

AnimationInfo ImageContainerTexturePack::getAnimation(int32_t groupIdx, int32_t directionIdx) const
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
