#pragma once

#include "ImageContainers/ImageContainer.h"
#include <map>
#include "TexturePack.h"
#include <vector>

class CachedTexturePack : public TexturePack
{
private:
	std::shared_ptr<ImageContainer> imgPack;
	std::shared_ptr<Palette> palette;
	bool indexed{ false };

	mutable std::map<size_t, sf::Texture> cache;

public:
	CachedTexturePack(const std::shared_ptr<ImageContainer>& imgPack_,
		const std::shared_ptr<Palette>& palette_, bool isIndexed_ = false)
		: imgPack(imgPack_), palette(palette_), indexed(isIndexed_) {}

	virtual bool get(size_t index,
		const sf::Texture**, sf::IntRect& textureRect) const;

	virtual bool get(size_t indexX, size_t indexY,
		const sf::Texture** texture, sf::IntRect& textureRect) const
	{
		return get(indexX, texture, textureRect);
	}

	virtual bool getTextureSize(sf::Vector2i& textureSize) const { return false; }

	virtual const std::shared_ptr<Palette>& getPalette() const { return palette; }
	virtual bool isIndexed() const { return indexed; }
	virtual size_t packSize() const { return 1; }
	virtual size_t totalSize() const { return imgPack->size(); }
	virtual size_t size(size_t index) const { return imgPack->size(); }
};

class CachedMultiTexturePack : public TexturePack
{
private:
	std::vector<std::shared_ptr<ImageContainer>> imgVec;
	size_t textureCount{ 0 };
	std::shared_ptr<Palette> palette;
	bool indexed{ false };

	mutable std::map<std::pair<size_t, size_t>, sf::Texture> cache;

public:
	CachedMultiTexturePack(const std::vector<std::shared_ptr<ImageContainer>>& imgVec_,
		const std::shared_ptr<Palette>& palette_, bool isIndexed_ = false);

	virtual bool get(size_t index,
		const sf::Texture** texture, sf::IntRect& textureRect) const;

	virtual bool get(size_t indexX, size_t indexY,
		const sf::Texture** texture, sf::IntRect& textureRect) const;

	virtual bool getTextureSize(sf::Vector2i& textureSize) const { return false; }

	virtual const std::shared_ptr<Palette>& getPalette() const { return palette; }
	virtual bool isIndexed() const { return indexed; }
	virtual size_t packSize() const { return imgVec.size(); }
	virtual size_t totalSize() const { return textureCount; }
	virtual size_t size(size_t index) const { return imgVec[index]->size(); }
};
