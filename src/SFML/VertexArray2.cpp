#include "VertexArray2.h"
#include <SFML/Graphics/VertexArray.hpp>
#include "Utils/Utils.h"

void VertexArray2::draw(const sf::Texture* texture, const Palette* palette,
	GameShader* spriteShader, sf::RenderTarget& target,
	sf::Transform transform, sf::Glsl::Vec2 pixelSize) const
{
	if (palette == nullptr)
	{
		spriteShader = nullptr;
	}

	sf::RenderStates states(sf::BlendAlpha, transform, texture, nullptr);

	if (spriteShader != nullptr)
	{
		auto shader = spriteShader->shader.get();
		states.shader = shader;
		for (auto uniformHash : spriteShader->uniforms)
		{
			switch (uniformHash)
			{
			case str2int16("pixelSize"):
			{
				shader->setUniform("pixelSize", pixelSize);
				break;
			}
			case str2int16("outline"):
			{
				shader->setUniform("outline", sf::Glsl::Vec4(sf::Color::Transparent));
				break;
			}
			case str2int16("ignore"):
			{
				shader->setUniform("ignore", sf::Glsl::Vec4(sf::Color::Transparent));
				break;
			}
			case str2int16("palette"):
			{
				shader->setUniform("hasPalette", true);
				shader->setUniform("palette", palette->texture);
				break;
			}
			default:
				break;
			}
		}
	}
	target.draw(vertices.data(), vertices.size(), sf::PrimitiveType::Triangles, states);
}

void VertexArray2::draw(const sf::Texture* texture, const Palette* palette,
	GameShader* spriteShader, sf::RenderTarget& target) const
{
	if (vertices.empty() == true)
	{
		return;
	}
	draw(texture, palette, spriteShader, target, sf::Transform::Identity, {});
}

void VertexArray2::draw(const sf::Texture* texture, const sf::Vector2f& pos,
	const sf::Vector2f& size, const Palette* palette,
	GameShader* spriteShader, sf::RenderTarget& target) const
{
	if (vertices.empty() == true)
	{
		return;
	}
	sf::Transform t;
	t.transformRect({ {}, size });
	t = t.translate(pos);
	draw(texture, palette, spriteShader, target, t,
		sf::Glsl::Vec2(
			1.0f / (float)size.x,
			1.0f / (float)size.y
		)
	);
}
