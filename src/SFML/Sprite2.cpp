#include "Sprite2.h"
#include "SFMLUtils.h"
#include "ShaderManager.h"

void Sprite2::setPosition(const sf::Vector2f& position_)
{
	position = position_;
	sf::Sprite::setPosition(position + offset);
}

void Sprite2::setPosition(const sf::Vector2f& position_, const sf::Vector2f& offset_)
{
	position = position_;
	offset = offset_;
	sf::Sprite::setPosition(position + offset);
}

void Sprite2::setOffset(const sf::Vector2f& offset_)
{
	offset = offset_;
	sf::Sprite::setPosition(position + offset);
}

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
	outlineEnabled = enable;
}

void Sprite2::setTexture(const sf::Texture& texture, bool resetRect)
{
	sf::Sprite::setTexture(texture, resetRect);
	blendMode = BlendMode::Alpha;
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
	blendMode = ti.blendMode;
	setPalette(ti.palette);
}

bool Sprite2::needsSpriteShader() const noexcept
{
	if (hasPalette() == false &&
		(isOutlineEnabled() == false || hasOutline() == false))
	{
		return false;
	}
	return true;
}

void Sprite2::draw(sf::RenderTarget& target, sf::Shader* spriteShader) const
{
	sf::RenderStates states(SFMLUtils::getBlendMode(blendMode));

	if (spriteShader != nullptr &&
		needsSpriteShader() == true)
	{
		states.shader = spriteShader;

		spriteShader->setUniform("pixelSize", sf::Glsl::Vec2(
			1.0f / (float)getTextureRect().width,
			1.0f / (float)getTextureRect().height
		));
		if (outlineEnabled == true)
		{
			spriteShader->setUniform("outline", sf::Glsl::Vec4(outline));
			spriteShader->setUniform("ignore", sf::Glsl::Vec4(ignore));
		}
		else
		{
			spriteShader->setUniform("outline", sf::Glsl::Vec4(sf::Color::Transparent));
			spriteShader->setUniform("ignore", sf::Glsl::Vec4(sf::Color::Transparent));
		}
		spriteShader->setUniform("hasPalette", hasPalette());
		if (hasPalette() == true)
		{
			spriteShader->setUniform("palette", palette->texture);
		}
	}
	target.draw(static_cast<sf::Sprite>(*this), states);
}

void Sprite2::draw(sf::RenderTarget& target, sf::Shader* spriteShader,
	SpriteShaderCache& cache) const
{
	sf::RenderStates states(SFMLUtils::getBlendMode(blendMode));

	if (spriteShader != nullptr &&
		needsSpriteShader() == true)
	{
		states.shader = spriteShader;

		bool updateAll = false;
		if (spriteShader != cache.shader)
		{
			cache.shader = spriteShader;
			updateAll = true;
		}

		if (updateAll == true ||
			getTextureRect().width != cache.textureSize.x ||
			getTextureRect().height != cache.textureSize.y)
		{
			cache.textureSize.x = getTextureRect().width;
			cache.textureSize.y = getTextureRect().height;
			spriteShader->setUniform("pixelSize", sf::Glsl::Vec2(
				1.0f / (float)getTextureRect().width,
				1.0f / (float)getTextureRect().height
			));
		}

		sf::Color outline2 = outline;
		sf::Color ignore2 = ignore;
		if (outlineEnabled == false)
		{
			outline2 = sf::Color::Transparent;
			ignore2 = sf::Color::Transparent;
		}

		if (updateAll == true ||
			outline2 != cache.outline)
		{
			cache.outline = outline2;
			spriteShader->setUniform("outline", sf::Glsl::Vec4(outline2));
		}

		if (updateAll == true ||
			ignore2 != cache.outline)
		{
			cache.ignore = ignore2;
			spriteShader->setUniform("ignore", sf::Glsl::Vec4(ignore2));
		}

		if (updateAll == true ||
			palette.get() != cache.palette)
		{
			cache.palette = palette.get();
			spriteShader->setUniform("hasPalette", hasPalette());
			if (hasPalette() == true)
			{
				spriteShader->setUniform("palette", palette->texture);
			}
		}
	}
	target.draw(static_cast<sf::Sprite>(*this), states);
}
