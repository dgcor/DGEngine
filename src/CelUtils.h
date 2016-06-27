#pragma once

#include <cstdint>
#include "Palette.h"

namespace CelUtils
{
	sf::Image loadImage(const char* fileName, const Palette& pal, bool isCl2);
	sf::Image loadImage(const char* fileName, const Palette& pal, bool isCl2,
		size_t& frameCountX, size_t& frameCountY);

	sf::Image loadImageFrame(const char* fileName, const Palette& pal, bool isCl2,
		size_t frameIdx);

	sf::Image loadBitmapFontImage(const char* fileName, const char* fileNameBin,
		const Palette& pal, bool isCl2);
}
