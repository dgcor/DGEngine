#pragma once

#include "Game/UIObject.h"
#include "SFML/CompositeSprite.h"

class Image : public virtual UIObject
{
protected:
	CompositeSprite sprite;
	Anchor anchor{ Anchor::Top | Anchor::Left };
	bool resizable{ false };
	bool stretch{ false };
	bool background{ false };
	bool visible{ true };

public:
	Image() noexcept;
	Image(const sf::Texture& tex);
	Image(const sf::Texture& tex, const std::shared_ptr<Palette>& pal);
	Image(const TextureInfo& ti);
	Image(const std::vector<TextureInfo>& ti, bool drawAfter = false);

	auto Resizable() const noexcept { return resizable; }
	void Resizable(auto resizable_) noexcept { resizable = resizable_; }

	auto Stretch() const noexcept { return stretch; }
	void Stretch(auto stretch_) noexcept { stretch = stretch_; }

	auto Background() const noexcept { return background; }
	void Background(auto background_) noexcept { background = background_; }

	//void scale(const sf::Vector2f& factor) { sprite.scale(factor); }
	const sf::Color getColor() { return sprite.getColor(); }
	void setColor(const auto& color) { sprite.setColor(color); }
	void setOrigin(const auto& origin) { sprite.setOrigin(origin); }
	void setOrigin();

	void setOutline(const auto& outline, const auto& ignore) noexcept
	{
		sprite.setOutline(outline, ignore);
	}
	auto hasOutline() const noexcept { return sprite.hasOutline(); }

	void setOutlineEnabled(auto enable) noexcept
	{
		sprite.setOutlineEnabled(enable);
	}
	auto isOutlineEnabled() const noexcept { return sprite.isOutlineEnabled(); }

	auto& getPalette() const noexcept { return sprite.getPalette(); }
	void setPalette(const auto& pal) noexcept { sprite.setPalette(pal); }
	auto hasPalette() const noexcept { return sprite.hasPalette(); }

	auto getTexture() const { return sprite.getTexture(); }
	auto& getTextureRect() const { return sprite.getTextureRect(); }

	void setTexture(const sf::Texture& texture, bool resetRect = false);
	void setTexture(const TextureInfo& ti);
	void setTexture(const std::vector<TextureInfo>& ti, bool drawAfter = false);
	void setTexture(const TextureInfoVar& ti);
	void setTextureRect(const sf::IntRect& rectangle) { sprite.setTextureRect(rectangle); }

	Anchor getAnchor() const noexcept override { return anchor; }
	void setAnchor(const Anchor anchor_) noexcept override { anchor = anchor_; }
	void updateSize(const Game& game) override;

	bool Visible() const noexcept override { return visible; }
	void Visible(bool visible_) noexcept override { visible = visible_; }

	const sf::Vector2f& DrawPosition() const override { return sprite.getDrawPosition(); }
	const sf::Vector2f& Position() const override { return sprite.getPosition(); }
	void Position(const sf::Vector2f& position_) override { return sprite.setPosition(position_); }
	sf::Vector2f Size() const override;
	void Size(const sf::Vector2f& size) override;
	void draw(const Game& game, sf::RenderTarget& target) const override;

	bool getProperty(const std::string_view prop, Variable& var) const override;
};
