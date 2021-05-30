#include "CachedImagePack.h"
#include <cassert>

CachedImagePack::CachedImagePack(const ImageContainer* imgContainer_,
	const std::shared_ptr<Palette>& palette_, bool isIndexed_)
	: imgContainer(imgContainer_), palette(palette_), isIndexed(isIndexed_)
{
	cache.resize(imgContainer_->size());

	if (isIndexed == false && palette == nullptr)
	{
		isIndexed = true;
	}
}

sf::Image& CachedImagePack::operator[] (uint32_t index)
{
	assert(index < imgContainer->size());
	if (cache[index].has_value() == false)
	{
		cache[index] = imgContainer->get(index,
			(isIndexed == true ? nullptr : &palette->palette));
	}
	return *cache[index];
}
