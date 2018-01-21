#pragma once

#include "ImageContainer.h"
#include <string>

class SimpleImageContainer : public ImageContainer
{
private:
	sf::Image image;
	size_t maxFrames{ 0 };
	size_t numFrames{ 0 };
	size_t subImageSizeX{ 0 };
	size_t subImageSizeY{ 0 };
	bool horizontalDirection{ false };

public:
	SimpleImageContainer(const std::string& fileName, size_t xFrames_,
		size_t yFrames_, bool horizontalDirection_ = false,
		const sf::Color& transparencyMask = sf::Color::Transparent);

	virtual sf::Image2 get(size_t index, const PaletteArray* palette) const;

	virtual size_t size() const noexcept { return numFrames; }
};
