#pragma once

#ifndef NO_DIABLO_FORMAT_SUPPORT
#include <cstdint>
#include "ImageContainer.h"
#include <string_view>

// CL2 decoding code based on Diablo 1 Graphics Tool by savagesteel
// https://github.com/savagesteel/d1-graphics-tool
class CL2ImageContainer : public ImageContainer
{
private:
	std::vector<uint8_t> fileData;
	std::vector<std::pair<uint32_t, uint32_t>> frameOffsets;
	uint32_t directions{ 0 };
	BlendMode blendMode{ BlendMode::Alpha };

public:
	CL2ImageContainer(const std::string_view fileName);

	virtual BlendMode getBlendMode() const noexcept { return blendMode; }
	virtual void setBlendMode(BlendMode blendMode_) noexcept { blendMode = blendMode_; }

	virtual sf::Image2 get(uint32_t index,
		const PaletteArray* palette, ImageInfo& imgInfo) const;

	virtual uint32_t size() const noexcept { return frameOffsets.size(); }

	virtual uint32_t getDirections() const noexcept { return directions; }
};
#endif
