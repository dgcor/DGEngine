#pragma once

#include <array>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "TexturePack.h"

class BitmapFontTexturePack : public TexturePack
{
private:
	std::shared_ptr<sf::Texture> texture;
	std::shared_ptr<Palette> palette;
	int rows{ 0 };
	int columns{ 0 };
	bool indexed{ false };
	std::array<sf::IntRect, 256> chars;

	void calculateCharSizes(const sf::Image& img, bool verticalDirection);

public:
	BitmapFontTexturePack(const std::shared_ptr<sf::Texture>& tex, int rows_, int columns_,
		bool verticalDirection, const sf::Image& img);
	BitmapFontTexturePack(const std::shared_ptr<sf::Texture>& tex, int rows_, int columns_,
		bool verticalDirection, const std::vector<uint8_t>& charSizes);

	int getCharWidth(char c) const noexcept { return chars[(unsigned char)c].width; }

	const sf::Texture& getTexture() const noexcept { return *texture.get(); }

	virtual bool get(size_t index, TextureInfo& ti) const;

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept { return palette; }
	virtual bool isIndexed() const noexcept { return indexed; }
	virtual size_t size() const noexcept { return chars.size(); }
};
