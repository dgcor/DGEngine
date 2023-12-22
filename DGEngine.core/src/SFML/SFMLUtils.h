#pragma once

#include "Game/BlendMode.h"
#include <optional>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <string_view>

namespace SFMLUtils
{
	sf::BlendMode getBlendMode(BlendMode blendMode);

	sf::Color rgbToColor(unsigned val);
	sf::Color rgbaToColor(unsigned val);
	std::optional<sf::Color> stringToColor(std::string_view str);

	void spriteCenterTexture(sf::Sprite& sprite);

	float getScaleToStretchAndKeepAR(const sf::Vector2u& size,
		const sf::Vector2u& windowSize);

	void viewStretchKeepAR(sf::View& view, const sf::Vector2u& windowSize,
		sf::FloatRect viewPort = sf::FloatRect(0, 0, 1, 1));
}
