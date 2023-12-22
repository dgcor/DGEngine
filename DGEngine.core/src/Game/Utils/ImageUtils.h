#pragma once

#include <memory>
#include "Resources/ImageContainer.h"
#include "Resources/Palette.h"
#include <SFML/System/InputStream.hpp>
#include <vector>

namespace ImageUtils
{
	void applyMask(sf::Image& image,
		const sf::Color& transparencyMask = sf::Color::Transparent);

	sf::Image loadImage(sf::InputStream& inputStream,
		const sf::Color& transparencyMask = sf::Color::Transparent);

	sf::Image loadImage(const std::string_view fileName,
		const sf::Color& transparencyMask = sf::Color::Transparent);

	sf::Image loadImage(const ImageContainer& imgContainer,
		const std::shared_ptr<Palette>& pal);

	sf::Image loadImageFrame(const ImageContainer& imgContainer,
		const PaletteArray* pal, uint32_t frameIdx);

	sf::Image loadBitmapFontImage(const ImageContainer& imgContainer,
		const std::string_view fileNameCharMap, const std::shared_ptr<Palette>& pal);

	sf::Image packImages(const ImageContainer& imgContainer,
		const std::shared_ptr<Palette>& pal, std::vector<sf::IntRect>* rects = nullptr);

	// splits a 2x2 image into a 1x4 image (2 pieces)
	sf::Image splitImageHorizontal(const sf::Image& img, unsigned pieces = 1);

	// splits a 2x2 image into a 4x1 image (2 pieces)
	sf::Image splitImageVertical(const sf::Image& img, unsigned pieces = 1);
}
