#pragma once

#include <SFML/Graphics/Color.hpp>

class LevelSurface;

struct ColorLevelLayer
{
	sf::Color background{ sf::Color::Transparent };

	ColorLevelLayer() {}
	ColorLevelLayer(sf::Color background_) : background(background_) {}

	void draw(const LevelSurface& surface) const;
};
