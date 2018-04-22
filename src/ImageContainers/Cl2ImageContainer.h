#pragma once

#include "CelBaseImageContainer.h"
#include "gsl/gsl"

class Cl2ImageContainer : public CelBaseImageContainer
{
private:
	static uint16_t computeWidthFromHeader(const gsl::span<const uint8_t> frameData);

	static sf::Image2 decode(const gsl::span<const uint8_t> frameData,
		const PaletteArray* palette);

public:
	Cl2ImageContainer(const char* fileName);

	virtual sf::Image2 get(size_t index, const PaletteArray * palette) const;
};
