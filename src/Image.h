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
	Image() noexcept
	{
		sprite.setOutlineEnabled(true);
	}
	Image(const sf::Texture& tex) : sprite(tex)
	{
		sprite.setOutlineEnabled(true);
	}
	Image(const sf::Texture& tex, const std::shared_ptr<Palette>& pal) : sprite(tex, pal)
	{
		sprite.setOutlineEnabled(true);
	}
	Image(const TextureInfo& ti) : sprite(ti)
	{
		sprite.setOutlineEnabled(true);
	}

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

	const sf::IntRect& getTextureRect() const { return sprite.getTextureRect(); }

	void setTexture(const TextureInfo& ti, bool resetRect = false);
	void setTexture(const sf::Texture& texture, bool resetRect = false);
	void setTextureRect(const sf::IntRect& rectangle) { sprite.setTextureRect(rectangle); }

	virtual Anchor getAnchor() const noexcept { return anchor; }
	virtual void setAnchor(const Anchor anchor_) noexcept { anchor = anchor_; }
	virtual void updateSize(const Game& game);

	virtual bool Visible() const noexcept { return visible; }
	virtual void Visible(bool visible_) noexcept { visible = visible_; }

	virtual const sf::Vector2f& DrawPosition() const { return sprite.getDrawPosition(); }
	virtual const sf::Vector2f& Position() const { return sprite.getPosition(); }
	virtual void Position(const sf::Vector2f& position_) { return sprite.setPosition(position_); }
	virtual sf::Vector2f Size() const;
	virtual void Size(const sf::Vector2f& size);
	virtual void draw(const Game& game, sf::RenderTarget& target) const;

	virtual bool getProperty(const std::string_view prop, Variable& var) const;
};
