#pragma once

#include <memory>
#include <SFML/Graphics/Image.hpp>

namespace ImageUtils
{
	sf::Image loadImage(const std::string& fileName, const sf::Color& color = sf::Color::Transparent);

	sf::Image splitImageHorizontal(const sf::Image& img, unsigned pieces = 1);
	sf::Image splitImageVertical(const sf::Image& img, unsigned pieces = 1);
}
