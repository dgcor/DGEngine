#pragma once

#include <memory>
#include "Palette.h"
#include <SFML/Graphics/Texture.hpp>

struct TextureInfo
{
	const sf::Texture* texture{ nullptr };
	sf::IntRect textureRect;
	sf::Vector2f offset;
	std::shared_ptr<Palette> palette;
};
