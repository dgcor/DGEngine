#pragma once

#include <memory>
#include "Palette.h"
#include <SFML/Graphics/Texture.hpp>

struct TextureInfo
{
	const sf::Texture* texture;
	sf::IntRect textureRect;
	sf::Vector2f offset;
};

class TexturePack
{
protected:
	static void updateTextureInfo(TextureInfo& ti);

public:
	virtual bool get(size_t index, TextureInfo& ti) const = 0;

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept = 0;
	virtual bool isIndexed() const noexcept = 0;
	virtual size_t size() const noexcept = 0;
};
