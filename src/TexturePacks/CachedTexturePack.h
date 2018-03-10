#pragma once

#include "ImageContainers/ImageContainer.h"
#include "TexturePack.h"
#include <vector>

class CachedTexturePack : public TexturePack
{
private:
	std::shared_ptr<ImageContainer> imgPack;
	std::shared_ptr<Palette> palette;
	bool indexed{ false };

	mutable std::vector<sf::Texture> cache;

public:
	CachedTexturePack(const std::shared_ptr<ImageContainer>& imgPack_,
		const std::shared_ptr<Palette>& palette_, bool isIndexed_);

	virtual bool get(size_t index, TextureInfo& ti) const;

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept { return palette; }
	virtual bool isIndexed() const noexcept { return indexed; }
	virtual size_t size() const noexcept { return cache.size(); }
};

class CachedMultiTexturePack : public TexturePack
{
private:
	std::vector<std::shared_ptr<ImageContainer>> imgVec;
	size_t textureCount{ 0 };
	std::shared_ptr<Palette> palette;
	bool indexed{ false };

	mutable std::vector<sf::Texture> cache;

public:
	CachedMultiTexturePack(const std::vector<std::shared_ptr<ImageContainer>>& imgVec_,
		const std::shared_ptr<Palette>& palette_, bool isIndexed_);

	virtual bool get(size_t index, TextureInfo& ti) const;

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept { return palette; }
	virtual bool isIndexed() const noexcept { return indexed; }
	virtual size_t size() const noexcept { return textureCount; }

	std::vector<std::pair<size_t, size_t>> getRanges() const;
};
