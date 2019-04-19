#pragma once

#include "LevelLayerInfo.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

struct ColorLevelLayer
{
	sf::Color background{ sf::Color::Transparent };

	ColorLevelLayer() {}
	ColorLevelLayer(sf::Color background_) : background(background_) {}

	void draw(sf::RenderTexture& levelTexture, const LevelLayerInfo& layerInfo) const;
};
