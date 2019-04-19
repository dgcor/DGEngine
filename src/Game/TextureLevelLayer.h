#pragma once

#include "LevelLayerInfo.h"
#include <memory>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "Utils/ElapsedTime.h"

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

	void draw(sf::RenderTexture& levelTexture,
		const LevelLayerInfo& layerInfo, const sf::Vector2f& viewCenter) const;
};
