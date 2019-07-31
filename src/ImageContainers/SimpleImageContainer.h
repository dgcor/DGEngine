#pragma once

#include "ImageContainer.h"
#include <string_view>

class SimpleImageContainer : public ImageContainer
{
private:
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

	virtual BlendMode getBlendMode() const noexcept { return blendMode; }
	virtual void setBlendMode(BlendMode blendMode_) noexcept { blendMode = blendMode_; }

	virtual sf::Image2 get(uint32_t index,
		const PaletteArray* palette, ImageInfo& imgInfo) const;

	virtual uint32_t size() const noexcept { return numFrames; }

	virtual uint32_t getDirections() const noexcept { return directions; }
};
