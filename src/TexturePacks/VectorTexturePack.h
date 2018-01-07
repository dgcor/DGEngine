#pragma once

#include "TexturePack.h"
#include <vector>

class VectorTexturePack : public TexturePack
{
private:
	std::vector<sf::Texture> textures;
	std::shared_ptr<Palette> palette;
	bool indexed{ false };
	size_t lastTextureIndex{ 0 };

	bool texturesHaveSameSize() const { return lastTextureIndex < textures.size(); }

public:
	VectorTexturePack(size_t size_, const std::shared_ptr<Palette>& palette_,
		bool isIndexed_ = false) : textures(size_), palette(palette_),
		indexed(isIndexed_), lastTextureIndex(size_) {}

	virtual bool get(size_t index,
		const sf::Texture** texture, sf::IntRect& textureRect) const;

	virtual bool get(size_t indexX, size_t indexY,
		const sf::Texture** texture, sf::IntRect& textureRect) const
	{
		return get(indexX, texture, textureRect);
	}

	virtual bool getTextureSize(sf::Vector2i& textureSize) const;

	void set(size_t index, const sf::Texture& texture);

	virtual const std::shared_ptr<Palette>& getPalette() const { return palette; }
	virtual bool isIndexed() const { return indexed; }
	virtual size_t packSize() const { return 1; }
	virtual size_t totalSize() const { return textures.size(); }
	virtual size_t size(size_t index) const { return textures.size(); }
};
