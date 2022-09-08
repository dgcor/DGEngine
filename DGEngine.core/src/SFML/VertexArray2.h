#pragma once

#include "Resources/Palette.h"
#include "Resources/Shader.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <vector>

class VertexArray2
{
private:
	void draw(const sf::Texture* texture, const Palette* palette,
		GameShader* spriteShader, sf::RenderTarget& target,
		sf::Transform transform, sf::Glsl::Vec2 pixelSize) const;

public:
	std::vector<sf::Vertex> vertices;

	void draw(const sf::Texture* texture, const Palette* palette,
		GameShader* spriteShader, sf::RenderTarget& target) const;

	void draw(const sf::Texture* texture, const sf::Vector2f& pos,
		const sf::Vector2f& size, const Palette* palette,
		GameShader* spriteShader, sf::RenderTarget& target) const;
};
