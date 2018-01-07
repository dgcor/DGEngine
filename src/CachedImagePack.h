#pragma once

#include "ImageContainers/ImageContainer.h"
#include <map>
#include <memory>

class CachedImagePack
{
private:
	const ImageContainer* imgContainer{ nullptr };
	std::shared_ptr<Palette> palette;
	bool isIndexed{ false };

	std::map<size_t, sf::Image> cache;

public:
	CachedImagePack() {}
	CachedImagePack(const ImageContainer* imgContainer_,
		const std::shared_ptr<Palette>& palette_, bool isIndexed_ = false)
		: imgContainer(imgContainer_), palette(palette_), isIndexed(isIndexed_) {}

	sf::Image& get(size_t index) { return (*this)[index]; }

	sf::Image& operator[] (size_t index)
	{
		if (cache.count(index))
		{
			return cache[index];
		}
		cache[index] = imgContainer->get(index,
			(isIndexed == true ? nullptr : &palette->palette));
		return cache[index];
	}

	const std::shared_ptr<Palette>& getPalette() const { return palette; }
	bool IsIndexed() const { return isIndexed; }
	size_t size() const { return imgContainer->size(); }
};
