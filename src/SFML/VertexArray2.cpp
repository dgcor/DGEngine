#include "VertexArray2.h"
#include <SFML/Graphics/VertexArray.hpp>

void VertexArray2::draw(const sf::Texture* texture, const Palette* palette,
	sf::Shader* spriteShader, sf::RenderTarget& target) const
{
	if (vertices.empty() == true)
	{
		return;
	}
	if (palette == nullptr)
	{
		spriteShader = nullptr;
	}
	if (spriteShader != nullptr)
	{
		spriteShader->setUniform("pixelSize", sf::Glsl::Vec2());
		spriteShader->setUniform("outline", sf::Glsl::Vec4(sf::Color::Transparent));
		spriteShader->setUniform("ignore", sf::Glsl::Vec4(sf::Color::Transparent));
		spriteShader->setUniform("hasPalette", true);
		spriteShader->setUniform("palette", palette->texture);
	}

	sf::RenderStates states(sf::BlendAlpha, sf::Transform::Identity, texture, spriteShader);

	target.draw(vertices.data(), vertices.size(), sf::PrimitiveType::Triangles, states);
}

void VertexArray2::draw(const sf::Texture* texture, const sf::Vector2f& pos,
	const sf::Vector2f& size, const Palette* palette,
	sf::Shader* spriteShader, sf::RenderTarget& target) const
{
	if (vertices.empty() == true)
	{
		return;
	}
	if (palette == nullptr)
	{
		spriteShader = nullptr;
	}
	if (spriteShader != nullptr)
	{
		spriteShader->setUniform("pixelSize", sf::Glsl::Vec2(
			1.0f / (float)size.x,
			1.0f / (float)size.y
		));
		spriteShader->setUniform("outline", sf::Glsl::Vec4(sf::Color::Transparent));
		spriteShader->setUniform("ignore", sf::Glsl::Vec4(sf::Color::Transparent));
		spriteShader->setUniform("hasPalette", true);
		spriteShader->setUniform("palette", palette->texture);
	}

	sf::Transform t;
	t.transformRect({ {}, size });
	t = t.translate(pos);
	sf::RenderStates states(sf::BlendAlpha, t, texture, spriteShader);

	target.draw(vertices.data(), vertices.size(), sf::PrimitiveType::Triangles, states);
}
