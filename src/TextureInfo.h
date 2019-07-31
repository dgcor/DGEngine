#pragma once

#include "BlendMode.h"
#include <memory>
#include "Palette.h"
#include <SFML/Graphics/Texture.hpp>

struct TextureInfo
{
	const sf::Texture* texture{ nullptr };
	sf::IntRect textureRect;
	sf::Vector2f offset;
	bool absoluteOffset{ false };
	BlendMode blendMode{ BlendMode::Alpha };
	std::shared_ptr<Palette> palette;
};
