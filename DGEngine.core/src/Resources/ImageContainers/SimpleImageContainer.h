#pragma once

#include "Resources/ImageContainer.h"
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
	bool verticalDirection{ false };
	BlendMode blendMode{ BlendMode::Alpha };

public:
	SimpleImageContainer(sf::InputStream& file, uint32_t xFrames_,
		uint32_t yFrames_, uint32_t directions_, bool verticalDirection_,
		const sf::Color& transparencyMask = sf::Color::Transparent);

	BlendMode getBlendMode() const noexcept override { return blendMode; }
	void setBlendMode(BlendMode blendMode_) noexcept override { blendMode = blendMode_; }

	sf::Image2 get(uint32_t index, const PaletteArray* palette, ImageInfo& imgInfo) const override;

	sf::Vector2u getImageSize(uint32_t index) const override;

	uint32_t size() const noexcept override { return numFrames; }

	uint32_t getDirections() const noexcept override { return directions; }
};
