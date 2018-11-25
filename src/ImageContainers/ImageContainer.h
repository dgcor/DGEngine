#pragma once

#include "Palette.h"
#include "SFML/Image2.h"

class ImageContainer
{
public:
	virtual ~ImageContainer() = default;
	virtual sf::Image2 get(size_t index, const PaletteArray* palette) const = 0;
	virtual size_t size() const noexcept = 0;
};
