#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace sf
{
	sf::Color rgbToColor(unsigned val);
	sf::Color rgbaToColor(unsigned val);

	void viewStretchKeepAR(sf::View& view, const sf::Vector2u& windowSize,
		sf::FloatRect viewPort = sf::FloatRect(0, 0, 1, 1));
}
