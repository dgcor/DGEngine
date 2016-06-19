#pragma once

#include "Cel.h"

template <class T>
class CelCache
{
private:
	const CelFile* cel;
	const Palette* palette;

	std::map<size_t, T> cache;

public:
	CelCache() {}
	CelCache(const CelFile& cel_, const Palette& palette_) : cel(&cel_), palette(&palette_) {}

	T& operator[] (size_t index)
	{
		if (cache.count(index))
		{
			return cache[index];
		}
		cache[index] = cel->get(index, *palette);
		return cache[index];
	}

	size_t size() const { return cel->Size(); }
};

template <class T>
class CelCacheVector
{
private:
	std::vector<const CelFile*> celVec;
	const Palette* palette;

	std::map<std::pair<size_t, size_t>, T> cache;

public:
	CelCacheVector() {}
	CelCacheVector(const std::vector<const CelFile*>& cel_, const Palette& palette_) : celVec(cel_), palette(&palette_) {}

	T& get(size_t celIdx, size_t frameIdx)
	{
		auto index = std::make_pair(celIdx, frameIdx);
		if (cache.count(index))
		{
			return cache[index];
		}
		cache[index] = celVec[celIdx]->get(frameIdx, *palette);
		return cache[index];
	}

	size_t size() const { return celVec.size(); }
	size_t size(size_t celIdx) const { return celVec[celIdx]->Size(); }
};

typedef CelCache<CelFrame> CelFrameCache;
typedef CelCache<sf::Image> CelImageCache;
typedef CelCache<sf::Texture> CelTextureCache;

typedef CelCacheVector<CelFrame> CelFrameCacheVector;
typedef CelCacheVector<sf::Image> CelImageCacheVector;
typedef CelCacheVector<sf::Texture> CelTextureCacheVector;
