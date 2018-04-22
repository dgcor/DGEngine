#pragma once

#include "TexturePack.h"
#include <vector>

class VectorTexturePack : public TexturePack
{
private:
	std::vector<sf::Texture> textures;
	sf::Vector2f offset;
	std::shared_ptr<Palette> palette;
	bool indexed{ false };
	size_t lastTextureIndex{ 0 };

	bool texturesHaveSameSize() const noexcept { return lastTextureIndex < textures.size(); }

public:
	VectorTexturePack(size_t size_, const std::shared_ptr<Palette>& palette_,
		bool isIndexed_) : textures(size_), palette(palette_),
		indexed(isIndexed_), lastTextureIndex(size_) {}

	virtual bool get(size_t index, TextureInfo& ti) const;

	void set(size_t index, const sf::Texture& texture);

	void setOffset(const sf::Vector2f& offset_) { offset = offset_; }

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept { return palette; }
	virtual bool isIndexed() const noexcept { return indexed; }
	virtual size_t size() const noexcept { return textures.size(); }
};
