#pragma once

#include "ImageContainers/ImageContainer.h"
#include <memory>
#include "Palette.h"
#include <SFML/System/InputStream.hpp>
#include <vector>

namespace ImageUtils
{
	void applyMask(sf::Image& image,
		const sf::Color& transparencyMask = sf::Color::Transparent);

	sf::Image loadImage(const std::string_view fileName,
		const sf::Color& transparencyMask = sf::Color::Transparent);

	sf::Image loadImage(const ImageContainer& imgContainer,
		const std::shared_ptr<Palette>& pal);

	sf::Image loadImageFrame(const ImageContainer& imgContainer,
		const PaletteArray* pal, uint32_t frameIdx);

	sf::Image loadBitmapFontImage(const ImageContainer& imgContainer,
		const std::string_view fileNameCharMap, const std::shared_ptr<Palette>& pal);

	sf::Image splitImageHorizontal(const sf::Image& img, unsigned pieces = 1);
	sf::Image splitImageVertical(const sf::Image& img, unsigned pieces = 1);
}
