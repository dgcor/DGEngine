#include "Sprite2.h"
#include "Shaders.h"

void Sprite2::setColor(const sf::Color& color)
{
	if (hasPalette() == false)
	{
		sf::Sprite::setColor(color);
	}
}

void Sprite2::setOutline(const sf::Color& outline_, const sf::Color& ignore_) noexcept
{
	outline = outline_;
	ignore = ignore_;
}

void Sprite2::setOutlineEnabled(bool enable) noexcept
{
	if (Shaders::supportsOutlines() == false)
	{
		return;
	}
	outlineEnabled = enable;
}

void Sprite2::setTexture(const TextureInfo& ti, bool resetRect)
{
	sf::Sprite::setTexture(*ti.texture);
	if (resetRect == true)
	{
		setTextureRect(ti.textureRect);
	}
	else
	{
		auto oldRect = getTextureRect();
		oldRect.left = ti.textureRect.left;
		oldRect.top = ti.textureRect.top;
		setTextureRect(oldRect);
	}
	setPalette(ti.palette);
}

void Sprite2::draw(sf::RenderTarget& target, sf::RenderStates states) const
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
