#pragma once

#include <SFML/Graphics/Rect.hpp>
#include "TexturePack.h"
#include <vector>

class BitmapFontTexturePack : public TexturePack
{
protected:
	std::shared_ptr<sf::Texture> texture;
	std::shared_ptr<Palette> palette;
	std::vector<sf::IntRect> charRects;

	void calculateCharSizes(const sf::Image& img, int rows, int columns,
		int16_t newLine, int16_t space, int16_t tab, bool verticalDirection);

public:
	BitmapFontTexturePack(const std::shared_ptr<sf::Texture>& tex,
		const std::shared_ptr<Palette>& palette_, int rows, int columns,
		int16_t newLine, int16_t space, int16_t tab, bool verticalDirection);

	BitmapFontTexturePack(const std::shared_ptr<sf::Texture>& tex,
		const std::shared_ptr<Palette>& palette_, int rows, int columns,
		int16_t newLine, int16_t space, int16_t tab, bool verticalDirection,
		const std::vector<uint8_t>& charSizes, size_t startPos, size_t skipNBytes);

	int32_t getWidth(uint32_t index) const noexcept override;

	const sf::Texture* getTexture() const noexcept override { return texture.get(); }

	bool get(uint32_t index, TextureInfo& ti) const override;

	const std::shared_ptr<Palette>& getPalette() const noexcept override { return palette; }
	uint32_t size() const noexcept override { return (uint32_t)charRects.size(); }
};
