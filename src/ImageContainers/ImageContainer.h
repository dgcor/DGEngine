#pragma once

#include "Image2.h"
#include "Palette.h"

class ImageContainer
{
public:
	virtual ~ImageContainer() {};
	virtual sf::Image2 get(size_t index, const PaletteArray* palette) const = 0;
	virtual size_t size() const noexcept = 0;
};
