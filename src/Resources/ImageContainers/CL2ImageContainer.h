#pragma once

#include "Resources/ImageContainers/LazyImageContainer.h"

// CL2 decoding code based on Diablo 1 Graphics Tool by savagesteel
// https://github.com/savagesteel/d1-graphics-tool
class CL2ImageContainer : public LazyImageContainer
{
private:
	uint32_t directions{ 0 };

	sf::Image2 decode(const std::span<const uint8_t> frameData, const PaletteArray* palette) const override;

public:
	CL2ImageContainer(const std::shared_ptr<FileBytes>& fileData_);

	uint32_t getDirections() const noexcept override { return directions; }
};
