#pragma once

#include <SFML/Graphics/Texture.hpp>
#include "TexturePack.h"
#include <vector>

class VectorTexturePack : public TexturePack
{
private:
	std::vector<sf::Texture> textures;
	sf::Vector2f offset;
	std::shared_ptr<Palette> palette;
	uint32_t lastTextureIndex{ 0 };

	bool texturesHaveSameSize() const noexcept { return lastTextureIndex < textures.size(); }

public:
	VectorTexturePack(uint32_t size_, const std::shared_ptr<Palette>& palette_)
		: textures(size_), palette(palette_), lastTextureIndex(size_) {}

	virtual bool get(uint32_t index, TextureInfo& ti) const;

	void set(uint32_t index, const sf::Texture& texture);

	void setOffset(const sf::Vector2f& offset_) { offset = offset_; }

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept { return palette; }
	virtual uint32_t size() const noexcept { return textures.size(); }
};
