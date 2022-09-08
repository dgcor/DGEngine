#pragma once

#include "Sprite2.h"
#include <vector>

class CompositeSprite
{
private:
	Sprite2 sprite;
	std::vector<Sprite2> extraSprites;
	bool drawAfterExtraSprites{ false };

public:
	CompositeSprite() = default;
	CompositeSprite(const sf::Texture& tex) : sprite(tex) {}
	CompositeSprite(const sf::Texture& tex, const std::shared_ptr<Palette>& pal) : sprite(tex, pal) {}
	CompositeSprite(const TextureInfo& ti);
	CompositeSprite(const std::vector<TextureInfo>& ti, bool drawAfter = false);

	auto& getDrawPosition() const { return sprite.getDrawPosition(); }
	auto& getPosition() const { return sprite.getPosition(); }
	auto getSize() const
	{
		return sf::Vector2f((float)sprite.getTextureRect().width, (float)sprite.getTextureRect().height);
	}
	void setPosition(const sf::Vector2f& position_);
	auto& getOffset() const { return sprite.getOffset(); }
	void setOffset(const sf::Vector2f& offset_);

	auto getLocalBounds() const { return sprite.getLocalBounds(); }
	auto getGlobalBounds() const { return sprite.getGlobalBounds(); }

	auto& getTextureRect() const { return sprite.getTextureRect(); }
	void setTextureRect(const sf::IntRect& rect) { sprite.setTextureRect(rect); }

	bool hasPalette() const noexcept { return sprite.hasPalette(); }
	auto& getPalette() const noexcept { return sprite.getPalette(); }
	void setPalette(const std::shared_ptr<Palette>& palette) noexcept { sprite.setPalette(palette); }

	void setColor(const sf::Color& color);
	void setOrigin(const sf::Vector2f& origin) { sprite.setOrigin(origin); }

	auto& getOutline() const noexcept { return sprite.getOutline(); }
	auto& getOutlineIgnore() const noexcept { return sprite.getOutlineIgnore(); }
	void setOutline(const sf::Color& outline_, const sf::Color& ignore_) noexcept;
	bool hasOutline() const noexcept { return sprite.hasOutline();}
	void setOutlineEnabled(bool enable) noexcept;
	bool isOutlineEnabled() const noexcept { return sprite.isOutlineEnabled(); }

	void setScale(const sf::Vector2f& factor) { sprite.setScale(factor); }

	auto getTexture() const { return sprite.getTexture(); }
	void setTexture(const sf::Texture& texture, bool resetRect = false);
	void setTexture(const TextureInfo& ti);
	void setTexture(const std::vector<TextureInfo>& ti, bool drawAfter = false);
	void setTexture(const TextureInfoVar& ti);

	void draw(sf::RenderTarget& target, GameShader* spriteShader) const;

	void draw(sf::RenderTarget& target, GameShader* spriteShader,
		SpriteShaderCache& cache) const;
};
