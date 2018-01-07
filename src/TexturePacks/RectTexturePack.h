#pragma once

#include "TexturePack.h"
#include <vector>

class RectTexturePack : public TexturePack
{
private:
	std::shared_ptr<sf::Texture> texture;
	std::vector<sf::IntRect> rects;
	std::shared_ptr<Palette> palette;
	bool indexed{ false };
	bool texturesHaveSameSize{ false };

public:
	RectTexturePack(const std::shared_ptr<sf::Texture>& texture_,
		const std::shared_ptr<Palette>& palette_ = nullptr, bool isIndexed_ = false)
		: texture(texture_), palette(palette_), indexed(isIndexed_) {}

	void add(const sf::IntRect& rect);

	virtual bool get(size_t index,
		const sf::Texture** textureOut, sf::IntRect& textureRect) const;

	virtual bool get(size_t indexX, size_t indexY,
		const sf::Texture** textureOut, sf::IntRect& textureRect) const;

	virtual bool getTextureSize(sf::Vector2i& textureSize) const;

	virtual const std::shared_ptr<Palette>& getPalette() const { return palette; }
	virtual bool isIndexed() const { return indexed; }
	virtual size_t packSize() const { return 1; }
	virtual size_t totalSize() const { return rects.size(); }
	virtual size_t size(size_t index) const { return rects.size(); }
};
