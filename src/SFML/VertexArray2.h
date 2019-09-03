#pragma once

#include "Palette.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <vector>

struct VertexArray2
{
	std::vector<sf::Vertex> vertices;

	void draw(const sf::Texture* texture, const Palette* palette,
		sf::Shader* spriteShader, sf::RenderTarget& target) const;

	void draw(const sf::Texture* texture, const sf::Vector2f& pos,
		const sf::Vector2f& size, const Palette* palette,
		sf::Shader* spriteShader, sf::RenderTarget& target) const;
};
