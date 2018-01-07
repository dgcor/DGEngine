#pragma once

#include <memory>
#include "Palette.h"
#include <SFML/Graphics/Texture.hpp>

class TexturePack
{
protected:
	static void updateTextureRect(const sf::Texture& texture, sf::IntRect& textureRect);

public:
	virtual bool get(size_t index,
		const sf::Texture** texture, sf::IntRect& textureRect) const = 0;
	virtual bool get(size_t indexX, size_t indexY,
		const sf::Texture** texture, sf::IntRect& textureRect) const = 0;

	virtual bool getTextureSize(sf::Vector2i& textureSize) const = 0;

	virtual size_t packSize() const = 0;
	virtual size_t totalSize() const = 0;
	virtual size_t size(size_t index) const = 0;

	virtual const std::shared_ptr<Palette>& getPalette() const = 0;
	virtual bool isIndexed() const = 0;
};
