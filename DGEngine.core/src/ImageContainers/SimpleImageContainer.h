#pragma once

#include "ImageContainer.h"
#include <string_view>

class SimpleImageContainer : public ImageContainer
{
protected:
	sf::Image image;
	uint32_t maxFrames{ 0 };
	uint32_t numFrames{ 0 };
	uint32_t subImageSizeX{ 0 };
	uint32_t subImageSizeY{ 0 };
	uint32_t directions{ 0 };
	bool horizontalDirection{ false };
	BlendMode blendMode{ BlendMode::Alpha };

public:
	SimpleImageContainer(const std::string_view fileName, uint32_t xFrames_,
		uint32_t yFrames_, uint32_t directions_, bool horizontalDirection_,
		const sf::Color& transparencyMask = sf::Color::Transparent);

	BlendMode getBlendMode() const noexcept override { return blendMode; }
	void setBlendMode(BlendMode blendMode_) noexcept override { blendMode = blendMode_; }

	sf::Image2 get(uint32_t index, const PaletteArray* palette, ImageInfo& imgInfo) const override;

	uint32_t size() const noexcept override { return numFrames; }

	uint32_t getDirections() const noexcept override { return directions; }
};
