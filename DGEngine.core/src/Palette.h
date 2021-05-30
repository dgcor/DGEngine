#pragma once

#include <array>
#include <SFML/Graphics/Texture.hpp>
#include <string_view>
#include <vector>

typedef std::array<sf::Color, 256> PaletteArray;

class Palette
{
private:
	void loadTexture();
	void updateTexture();

public:
	sf::Texture texture;
	PaletteArray palette;

	enum class ColorFormat
	{
		RGB,
		BGR,
		RGBA,
		BGRA,
		ARGB,
		ABGR,
	};

	Palette() noexcept {}
	Palette(const std::string_view  file, ColorFormat colorFormat);
	Palette(const Palette& pal, const std::vector<sf::Uint8> trn,
		size_t start, size_t length);

	const sf::Color& operator[](size_t index) const noexcept { return palette[index]; }

	bool shiftLeft(size_t shift, size_t startIdx, size_t stopIdx);
	bool shiftRight(size_t shift, size_t startIdx, size_t stopIdx);

	bool replaceRange(const Palette& srcPal, size_t srcStartIdx,
		size_t size, size_t dstStartIdx);
};
