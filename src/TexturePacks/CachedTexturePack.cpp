#include "CachedTexturePack.h"

bool CachedTexturePack::get(size_t index,
	const sf::Texture** texture, sf::IntRect& textureRect) const
{
	if (index >= imgPack->size())
	{
		return false;
	}
	if (cache.count(index) == 0)
	{
		cache[index] = imgPack->get(index,
			(indexed == true ? nullptr : &palette->palette));
	}
	(*texture) = &cache[index];
	updateTextureRect(**texture, textureRect);
	return true;
}

CachedMultiTexturePack::CachedMultiTexturePack(
	const std::vector<std::shared_ptr<ImageContainer>>& imgVec_,
	const std::shared_ptr<Palette>& palette_, bool isIndexed_)
	: imgVec(imgVec_), palette(palette_), indexed(isIndexed_)
{
	for (const auto& imgPack : imgVec_)
	{
		textureCount += imgPack->size();
	}
}

bool CachedMultiTexturePack::get(size_t index,
	const sf::Texture** texture, sf::IntRect& textureRect) const
{
	if (imgVec.empty() == true ||
		index >= textureCount)
	{
		return false;
	}

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
	return get(indexX, indexY, texture, textureRect);
}

bool CachedMultiTexturePack::get(size_t indexX, size_t indexY,
	const sf::Texture** texture, sf::IntRect& textureRect) const
{
	if (indexY >= imgVec.size() ||
		indexX >= imgVec[indexY]->size())
	{
		return false;
	}
	auto index = std::make_pair(indexX, indexY);
	if (cache.count(index) == 0)
	{
		cache[index] = imgVec[indexY]->get(indexX,
			(indexed == true ? nullptr : &palette->palette));
	}
	(*texture) = &cache[index];
	updateTextureRect(**texture, textureRect);
	return true;
}
