#pragma once

#include <memory>
#include "Palette.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Shaders.h"

class Sprite2 : public sf::Sprite
{
private:
	std::shared_ptr<Palette> palette;
	sf::Color outline{ sf::Color::Transparent };
	sf::Color ignore{ sf::Color::Transparent };
	bool outlineEnabled{ false };

public:
	Sprite2() noexcept : sf::Sprite() {}
	explicit Sprite2(const sf::Texture& texture,
		const std::shared_ptr<Palette>& palette_ = nullptr)
		: sf::Sprite(texture), palette(palette_) {}

	void setColor(const sf::Color& color)
	{
		if (hasPalette() == false)
		{
			sf::Sprite::setColor(color);
		}
	}

	void setPalette(const std::shared_ptr<Palette>& pal) noexcept { palette = pal; }
	bool hasPalette() const noexcept { return palette != nullptr; }

	void setOutline(const sf::Color& outline_, const sf::Color& ignore_) noexcept
	{
		outline = outline_;
		ignore = ignore_;
	}
	bool hasOutline() const noexcept { return outline.a > 0; }
	void setOutlineEnabled(bool enable) noexcept
	{
		if (Shaders::supportsOutlines() == false)
		{
			return;
		}
		outlineEnabled = enable;
	}
	bool isOutlineEnabled() const noexcept { return outlineEnabled; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (outlineEnabled == true &&
			hasOutline() == true)
		{
			states.shader = &Shaders::Outline;
			Shaders::Outline.setUniform("texWidth", 1.0f / (float)getTextureRect().width);
			Shaders::Outline.setUniform("texHeight", 1.0f / (float)getTextureRect().height);
			Shaders::Outline.setUniform("outline", sf::Glsl::Vec4(outline));
			Shaders::Outline.setUniform("ignore", sf::Glsl::Vec4(ignore));

			target.draw(static_cast<sf::Sprite>(*this), states);
		}
		if (hasPalette() == true)
		{
			states.shader = &Shaders::Palette;
			Shaders::Palette.setUniform("palette", palette->texture);
		}
		target.draw(static_cast<sf::Sprite>(*this), states);
	}
};
