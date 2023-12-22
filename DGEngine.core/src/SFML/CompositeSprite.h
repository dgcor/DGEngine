#pragma once

#include "Sprite2.h"
#include "Utils/small_vector.hpp"

class CompositeSprite
{
private:
	gch::small_vector<Sprite2, 1> sprites{ 1 };
	bool drawAfterExtraSprites{ false };

public:
	CompositeSprite() = default;
	CompositeSprite(const sf::Texture& tex) : sprites(1, Sprite2(tex)) {}
	CompositeSprite(const sf::Texture& tex, const std::shared_ptr<Palette>& pal) : sprites(1, Sprite2(tex, pal)) {}
	CompositeSprite(const TextureInfo& ti);
	CompositeSprite(const std::vector<TextureInfo>& ti, bool drawAfter = false);

	auto& getDrawPosition() const { return sprites.front().getDrawPosition(); }
	auto& getPosition() const { return sprites.front().getPosition(); }
	auto getSize() const
	{
		return sf::Vector2f((float)sprites.front().getTextureRect().width, (float)sprites.front().getTextureRect().height);
	}
	void setPosition(const sf::Vector2f& position_);
	auto& getOffset() const { return sprites.front().getOffset(); }
	void setOffset(const sf::Vector2f& offset_);

	auto getLocalBounds() const { return sprites.front().getLocalBounds(); }
	auto getGlobalBounds() const { return sprites.front().getGlobalBounds(); }

	auto& getTextureRect() const { return sprites.front().getTextureRect(); }
	void setTextureRect(const sf::IntRect& rect) { sprites.front().setTextureRect(rect); }

	bool hasPalette() const noexcept { return sprites.front().hasPalette(); }
	auto& getPalette() const noexcept { return sprites.front().getPalette(); }
	void setPalette(const std::shared_ptr<Palette>& palette) noexcept { sprites.front().setPalette(palette); }

	const sf::Color& getColor() const { return sprites.front().getColor(); }
	void setColor(const sf::Color& color);
	void setOrigin(const sf::Vector2f& origin) { sprites.front().setOrigin(origin); }

	auto& getOutline() const noexcept { return sprites.front().getOutline(); }
	auto& getOutlineIgnore() const noexcept { return sprites.front().getOutlineIgnore(); }
	void setOutline(const sf::Color& outline_, const sf::Color& ignore_) noexcept;
	bool hasOutline() const noexcept { return sprites.front().hasOutline();}
	void setOutlineEnabled(bool enable) noexcept;
	bool isOutlineEnabled() const noexcept { return sprites.front().isOutlineEnabled(); }

	void setScale(const sf::Vector2f& factor) { sprites.front().setScale(factor); }

	auto getTexture() const { return sprites.front().getTexture(); }
	void setTexture(const sf::Texture& texture, bool resetRect = false);
	void setTexture(const TextureInfo& ti);
	void setTexture(const std::vector<TextureInfo>& ti, bool drawAfter = false);
	void setTexture(const TextureInfoVar& ti);

	void draw(sf::RenderTarget& target, GameShader* spriteShader) const;

	void draw(sf::RenderTarget& target, GameShader* spriteShader,
		SpriteShaderCache& cache) const;
};
