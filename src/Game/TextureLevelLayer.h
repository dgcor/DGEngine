#pragma once

#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include "Utils/ElapsedTime.h"

class LevelSurface;

struct TextureLevelLayer
{
	std::shared_ptr<sf::Texture> texture;
	sf::IntRect textureRect;
	float parallaxSpeed{ 0.f };
	sf::Vector2f parallaxFixedSpeed;
	sf::Vector2f parallaxFixedSpeedOffset;
	ElapsedTime parallaxElapsedTime;

	TextureLevelLayer() {}
	TextureLevelLayer(const std::shared_ptr<sf::Texture>& texture_)
		: texture(texture_) {}

	void update(sf::Time elapsedTime_);

	void draw(const LevelSurface& surface) const;
};
