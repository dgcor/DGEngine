#pragma once

#include "Sprite2.h"
#include "UIObject.h"

class Image : public virtual UIObject
{
protected:
	Sprite2 sprite;
	Anchor anchor{ Anchor::Top | Anchor::Left };
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

	void scale(const sf::Vector2f& factor) { sprite.scale(factor); }
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

	void setPalette(const std::shared_ptr<Palette>& pal) noexcept { sprite.setPalette(pal); }
	bool hasPalette() const noexcept { return sprite.hasPalette(); }

	void centerTexture();

	const sf::IntRect& getTextureRect() const { return sprite.getTextureRect(); }

	void setTexture(const sf::Texture& texture, bool resetRect = false) { sprite.setTexture(texture, resetRect); }
	void setTextureRect(const sf::IntRect& rectangle) { sprite.setTextureRect(rectangle); }

	virtual void setAnchor(const Anchor anchor_) noexcept { anchor = anchor_; }
	virtual void updateSize(const Game& game);

	virtual bool Visible() const noexcept { return visible; }
	virtual void Visible(bool visible_) noexcept { visible = visible_; }

	virtual const sf::Vector2f& DrawPosition() const { return sprite.getPosition(); }
	virtual const sf::Vector2f& Position() const { return sprite.getPosition(); }
	virtual void Position(const sf::Vector2f& position) { sprite.setPosition(position); }
	virtual sf::Vector2f Size() const
	{
		return sf::Vector2f((float)sprite.getTextureRect().width, (float)sprite.getTextureRect().height);
	}
	virtual void Size(const sf::Vector2f& size)
	{
		auto rect = sprite.getTextureRect();
		rect.width = (int)size.x;
		rect.height = (int)size.y;
		sprite.setTextureRect(rect);
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (visible)
		{
			target.draw(sprite, states);
		}
	}

	virtual bool getProperty(const std::string& prop, Variable& var) const;
};
