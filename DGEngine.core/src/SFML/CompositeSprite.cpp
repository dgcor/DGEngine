#include "CompositeSprite.h"

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
	sprite.setPosition(position_);
	for (auto& s : extraSprites)
	{
		s.setPosition(position_);
	}
}

void CompositeSprite::setOffset(const sf::Vector2f& offset_)
{
	if (extraSprites.empty() == true)
	{
		sprite.setOffset(offset_);
	}
	else
	{
		auto diff = offset_ - sprite.getOffset();
		sprite.setOffset(offset_);
		for (auto& s : extraSprites)
		{
			s.setOffset(s.getOffset() + diff);
		}
	}
}

void CompositeSprite::setColor(const sf::Color& color)
{
	sprite.setColor(color);
	for (auto& s : extraSprites)
	{
		s.setColor(color);
	}
}

void CompositeSprite::setOutline(const sf::Color& outline_, const sf::Color& ignore_) noexcept
{
	sprite.setOutline(outline_, ignore_);
	for (auto& s : extraSprites)
	{
		s.setOutline(outline_, ignore_);
	}
}

void CompositeSprite::setOutlineEnabled(bool enable) noexcept
{
	sprite.setOutlineEnabled(enable);
	for (auto& s : extraSprites)
	{
		s.setOutlineEnabled(enable);
	}
}

void CompositeSprite::setTexture(const sf::Texture& texture, bool resetRect)
{
	sprite.setOffset({});
	sprite.setTexture(texture, resetRect);
	extraSprites.clear();
}

void CompositeSprite::setTexture(const TextureInfo& ti)
{
	sprite.setOffset(ti.offset);
	sprite.setTexture(ti, true);
	extraSprites.clear();
}

void CompositeSprite::setTexture(const std::vector<TextureInfo>& ti, bool drawAfter)
{
	if (ti.empty() == false)
	{
		drawAfterExtraSprites = drawAfter;
		sprite.setOffset(ti.front().offset);
		sprite.setTexture(ti.front(), true);
		extraSprites.resize(ti.size() - 1);
		for (size_t i = 0; i < extraSprites.size(); i++)
		{
			extraSprites[i].setPosition(sprite.getPosition(), ti[i + 1].offset);
			extraSprites[i].setTexture(ti[i + 1], true);
			extraSprites[i].setColor(sprite.getColor());
			extraSprites[i].setOutline(sprite.getOutline(), sprite.getOutlineIgnore());
			extraSprites[i].setOutlineEnabled(sprite.isOutlineEnabled());
		}
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
		sprite.draw(target, spriteShader, &cache);
	}
	for (const auto& s : extraSprites)
	{
		s.draw(target, spriteShader, &cache);
	}
	if (drawAfterExtraSprites == true)
	{
		sprite.draw(target, spriteShader, &cache);
	}
}
