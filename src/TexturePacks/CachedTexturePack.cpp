#include "CachedTexturePack.h"

CachedTexturePack::CachedTexturePack(const std::shared_ptr<ImageContainer>& imgPack_,
	const sf::Vector2f& offset_, const std::shared_ptr<Palette>& palette_, bool isIndexed_)
	: imgPack(imgPack_), offset(offset_), palette(palette_), indexed(isIndexed_)
{
	cache.resize(imgPack_->size());
}

bool CachedTexturePack::get(size_t index, TextureInfo& ti) const
{
	if (index >= imgPack->size())
	{
		return false;
	}
	if (cache[index].getNativeHandle() == 0)
	{
		cache[index] = imgPack->get(index,
			(indexed == true ? nullptr : &palette->palette));
	}
	ti.texture = &cache[index];
	updateTextureRect(ti);
	ti.offset = offset;
	ti.palette = palette;
	return true;
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

bool CachedMultiTexturePack::get(size_t index, TextureInfo& ti) const
{
	if (imgVec.empty() == true ||
		index >= textureCount)
	{
		return false;
	}
	if (cache[index].getNativeHandle() == 0)
	{
		size_t indexX = index;
		size_t indexY = 0;
		while (indexX >= imgVec[indexY]->size())
		{
			indexX -= imgVec[indexY]->size();
			indexY++;
			if (indexY >= imgVec.size())
			{
				return false;
			}
		}
		cache[index] = imgVec[indexY]->get(indexX,
			(indexed == true ? nullptr : &palette->palette));
	}
	ti.texture = &cache[index];
	updateTextureRect(ti);
	ti.offset = offset;
	ti.palette = palette;
	return true;
}

std::vector<std::pair<size_t, size_t>> CachedMultiTexturePack::getRanges() const
{
	std::vector<std::pair<size_t, size_t>> ranges;
	size_t startIdx = 0;
	size_t stopIdx = 0;
	for (const auto& imgCont : imgVec)
	{
		stopIdx += imgCont->size();
		ranges.push_back(std::make_pair(startIdx, stopIdx));
		startIdx = stopIdx;
	}
	return ranges;
}
