#pragma once

#include "SFML/CompositeSprite.h"
#include "UIObject.h"

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

	bool Resizable() const noexcept { return resizable; }
	void Resizable(bool resizable_) noexcept { resizable = resizable_; }

	bool Stretch() const noexcept { return stretch; }
	void Stretch(bool stretch_) noexcept { stretch = stretch_; }

	bool Background() const noexcept { return background; }
	void Background(bool background_) noexcept { background = background_; }

	//void scale(const sf::Vector2f& factor) { sprite.scale(factor); }
	void setColor(const sf::Color& color) { sprite.setColor(color); }
	void setOrigin(const sf::Vector2f& origin) { sprite.setOrigin(origin); }
	void setOrigin();

	void setOutline(const sf::Color& outline, const sf::Color& ignore) noexcept
	{
		sprite.setOutline(outline, ignore);
	}
	bool hasOutline() const noexcept { return sprite.hasOutline(); }

	void setOutlineEnabled(bool enable) noexcept
	{
		sprite.setOutlineEnabled(enable);
	}
	bool isOutlineEnabled() const noexcept { return sprite.isOutlineEnabled(); }

	const std::shared_ptr<Palette>& getPalette() const noexcept { return sprite.getPalette(); }
	void setPalette(const std::shared_ptr<Palette>& pal) noexcept { sprite.setPalette(pal); }
	bool hasPalette() const noexcept { return sprite.hasPalette(); }

	const sf::Texture* getTexture() const { return sprite.getTexture(); }
	const sf::IntRect& getTextureRect() const { return sprite.getTextureRect(); }

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
