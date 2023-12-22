#include "CompositeSprite.h"
#include <ranges>

CompositeSprite::CompositeSprite(const TextureInfo& ti)
{
	setTexture(ti);
}

CompositeSprite::CompositeSprite(const std::vector<TextureInfo>& ti, bool drawAfter)
{
	setTexture(ti, drawAfter);
}

void CompositeSprite::setPosition(const sf::Vector2f& position_)
{
	for (auto& sprite : sprites)
	{
		sprite.setPosition(position_);
	}
}

void CompositeSprite::setOffset(const sf::Vector2f& offset_)
{
	if (sprites.size() == 1)
	{
		sprites.front().setOffset(offset_);
	}
	else
	{
		auto diff = offset_ - sprites.front().getOffset();
		for (auto& sprite : sprites)
		{
			sprite.setOffset(sprite.getOffset() + diff);
		}
	}
}

void CompositeSprite::setColor(const sf::Color& color)
{
	for (auto& sprite : sprites)
	{
		sprite.setColor(color);
	}
}

void CompositeSprite::setOutline(const sf::Color& outline_, const sf::Color& ignore_) noexcept
{
	sprites.front().setOutline(outline_, ignore_);
	for (auto& sprite : sprites)
	{
		sprite.setOutline(outline_, ignore_);
	}
}

void CompositeSprite::setOutlineEnabled(bool enable) noexcept
{
	sprites.front().setOutlineEnabled(enable);
	for (auto& sprite : sprites)
	{
		sprite.setOutlineEnabled(enable);
	}
}

void CompositeSprite::setTexture(const sf::Texture& texture, bool resetRect)
{
	sprites.resize(1);
	sprites.front().setOffset({});
	sprites.front().setTexture(texture, resetRect);
}

void CompositeSprite::setTexture(const TextureInfo& ti)
{
	sprites.resize(1);
	sprites.front().setOffset(ti.offset);
	sprites.front().setTexture(ti, true);
}

void CompositeSprite::setTexture(const std::vector<TextureInfo>& ti, bool drawAfter)
{
	if (ti.empty() == true)
	{
		return;
	}
	drawAfterExtraSprites = drawAfter;
	sprites.resize(ti.size());
	sprites.front().setOffset(ti.front().offset);
	sprites.front().setTexture(ti.front(), true);
	for (size_t i = 1; auto& sprite : sprites | std::views::drop(1))
	{
		sprite.setPosition(sprites.front().getPosition(), ti[i].offset);
		sprite.setTexture(ti[i], true);
		sprite.setColor(sprites.front().getColor());
		sprite.setOutline(sprites.front().getOutline(), sprites.front().getOutlineIgnore());
		sprite.setOutlineEnabled(sprites.front().isOutlineEnabled());
		i++;
	}
}

void CompositeSprite::setTexture(const TextureInfoVar& ti)
{
	if (std::holds_alternative<TextureInfo>(ti) == true)
	{
		setTexture(std::get<TextureInfo>(ti));
	}
	else
	{
		setTexture(std::get<std::vector<TextureInfo>>(ti));
	}
}

void CompositeSprite::draw(sf::RenderTarget& target, GameShader* spriteShader) const
{
	SpriteShaderCache cache;
	draw(target, spriteShader, cache);
}

void CompositeSprite::draw(sf::RenderTarget& target, GameShader* spriteShader,
	SpriteShaderCache& cache) const
{
	if (drawAfterExtraSprites == false)
	{
		for (const auto& sprite : sprites)
		{
			sprite.draw(target, spriteShader, &cache);
		}
		return;
	}
	for (const auto& sprite : sprites | std::views::drop(1))
	{
		sprite.draw(target, spriteShader, &cache);
	}
	sprites.front().draw(target, spriteShader, &cache);
}
