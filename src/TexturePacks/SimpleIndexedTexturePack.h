#pragma once

#include <map>
#include "SimpleTexturePack.h"

class SimpleIndexedMultiTexturePack : public SimpleMultiTexturePack
{
	std::map<size_t, size_t> textureIndexes;
	size_t numIndexedTextures{ 0 };

public:
	SimpleIndexedMultiTexturePack(const std::shared_ptr<Palette>& palette_,
		bool isIndexed_ = false) : SimpleMultiTexturePack(palette_, isIndexed_) {}

	virtual bool get(size_t index,
		const sf::Texture** texture, sf::IntRect& textureRect) const;

	virtual bool get(size_t indexX, size_t indexY,
		const sf::Texture** texture, sf::IntRect& textureRect) const;

	void addTextureIndex(size_t index);

	virtual size_t packSize() const noexcept { return 1; }
	virtual size_t totalSize() const noexcept { return numIndexedTextures; }
	virtual size_t size(size_t index) const noexcept { return numIndexedTextures; }
};
