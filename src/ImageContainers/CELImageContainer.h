#pragma once

#ifndef NO_DIABLO_FORMAT_SUPPORT
#include <cstdint>
#include "ImageContainer.h"
#include <string_view>

// CEL decoding code based on Diablo 1 Graphics Tool by savagesteel
// https://github.com/savagesteel/d1-graphics-tool
class CELImageContainer : public ImageContainer
{
private:
	enum class CelType
	{
		None,
		V1Regular,
		V1Compilation,
		V1Level
	};

	CelType type{ CelType::None };
	std::vector<uint8_t> fileData;
	std::vector<std::pair<uint32_t, uint32_t>> frameOffsets;
	uint32_t directions{ 0 };
	BlendMode blendMode{ BlendMode::Alpha };

public:
	CELImageContainer(const std::string_view fileName);

	BlendMode getBlendMode() const noexcept override { return blendMode; }
	void setBlendMode(BlendMode blendMode_) noexcept override { blendMode = blendMode_; }

	sf::Image2 get(uint32_t index, const PaletteArray * palette, ImageInfo& imgInfo) const override;

	uint32_t size() const noexcept override { return frameOffsets.size(); }

	uint32_t getDirections() const noexcept override { return directions; }
};
#endif
