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

	const sf::Vector2f& getDrawPosition() const { return sprite.getDrawPosition(); }
	const sf::Vector2f& getPosition() const { return sprite.getPosition(); }
	sf::Vector2f getSize() const
	{
		return sf::Vector2f((float)sprite.getTextureRect().width, (float)sprite.getTextureRect().height);
	}
	void setPosition(const sf::Vector2f& position_);
	const sf::Vector2f& getOffset() const { return sprite.getOffset(); }
	void setOffset(const sf::Vector2f& offset_);

	sf::FloatRect getLocalBounds() const { return sprite.getLocalBounds(); }
	sf::FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); }

	const sf::IntRect& getTextureRect() const { return sprite.getTextureRect(); }
	void setTextureRect(const sf::IntRect& rect) { sprite.setTextureRect(rect); }

	bool hasPalette() const noexcept { return sprite.hasPalette(); }
	const std::shared_ptr<Palette>& getPalette() const noexcept { return sprite.getPalette(); }
	void setPalette(const std::shared_ptr<Palette>& palette) noexcept { sprite.setPalette(palette); }

	void setColor(const sf::Color& color);
	void setOrigin(const sf::Vector2f& origin) { sprite.setOrigin(origin); }

	const sf::Color& getOutline() const noexcept { return sprite.getOutline(); }
	const sf::Color& getOutlineIgnore() const noexcept { return sprite.getOutlineIgnore(); }
	void setOutline(const sf::Color& outline_, const sf::Color& ignore_) noexcept;
	bool hasOutline() const noexcept { return sprite.hasOutline();}
	void setOutlineEnabled(bool enable) noexcept;
	bool isOutlineEnabled() const noexcept { return sprite.isOutlineEnabled(); }

	void setScale(const sf::Vector2f factor) { sprite.setScale(factor); }

	const sf::Texture* getTexture() const { return sprite.getTexture(); }
	void setTexture(const sf::Texture& texture, bool resetRect = false);
	void setTexture(const TextureInfo& ti);
	void setTexture(const std::vector<TextureInfo>& ti, bool drawAfter = false);
	void setTexture(const TextureInfoVar& ti);

	void draw(sf::RenderTarget& target, GameShader* spriteShader) const;

	void draw(sf::RenderTarget& target, GameShader* spriteShader,
		SpriteShaderCache& cache) const;
};
