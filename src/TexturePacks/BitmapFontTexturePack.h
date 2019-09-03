#pragma once

#include <array>
#include "BlendMode.h"
#include <SFML/Graphics/Rect.hpp>
#include "TexturePack.h"

class BitmapFontTexturePack : public TexturePack
{
private:
	std::shared_ptr<sf::Texture> texture;
	std::shared_ptr<Palette> palette;
	int rows{ 0 };
	int columns{ 0 };
	std::array<sf::IntRect, 256> chars;

	void calculateCharSizes(const sf::Image& img, bool verticalDirection);

public:
	BitmapFontTexturePack(const std::shared_ptr<sf::Texture>& tex,
		const std::shared_ptr<Palette>& palette_, int rows_, int columns_,
		bool verticalDirection, const sf::Image& img);

	BitmapFontTexturePack(const std::shared_ptr<sf::Texture>& tex,
		const std::shared_ptr<Palette>& palette_, int rows_, int columns_,
		bool verticalDirection, const std::vector<uint8_t>& charSizes,
		size_t startPos, size_t skipNBytes, uint8_t spaceSize, uint8_t newLineSize);

	int getCharWidth(char c) const noexcept { return chars[(unsigned char)c].width; }

	virtual const sf::Texture* getTexture() const noexcept { return texture.get(); }

	virtual bool get(uint32_t index, TextureInfo& ti) const;

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept { return palette; }
	virtual uint32_t size() const noexcept { return chars.size(); }
};
