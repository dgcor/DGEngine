#pragma once

#include <cstdint>
#include "Cel.h"
#include "Palette.h"

namespace CelUtils
{
	sf::Image loadImage(const CelFile& celFile, const Palette& pal);
	sf::Image loadImage(const CelFile& celFile, const Palette& pal,
		size_t& frameCountX, size_t& frameCountY);

	sf::Image loadImageFrame(const CelFile& celFile, const Palette& pal,
		size_t frameIdx);

	sf::Image loadBitmapFontImage(const CelFile& celFile, const char* fileNameBin,
		const Palette& pal);
}
