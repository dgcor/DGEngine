#pragma once

#include <SFML/Graphics.hpp>
#include "UIObject.h"

class Image : public UIObject
{
private:
	sf::Sprite sprite;
	Anchor anchor{ Anchor::Top | Anchor::Left };
	bool visible{ true };

public:
	Image(const sf::Texture& tex) : sprite(tex) {}

	void scale(const sf::Vector2f& factor) { sprite.scale(factor); }
	void setColor(const sf::Color& color) { sprite.setColor(color); }
	void setOrigin(const sf::Vector2f& origin) { sprite.setOrigin(origin); }
	void setOrigin();

	void centerTexture();

	const sf::IntRect& getTextureRect() const { return sprite.getTextureRect(); }

	void setTexture(const sf::Texture& texture, bool resetRect = false) { sprite.setTexture(texture, resetRect); }
	void setTextureRect(const sf::IntRect& rectangle) { sprite.setTextureRect(rectangle); }

	virtual void setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) {}

	virtual void setAnchor(const Anchor anchor_) { anchor = anchor_; }
	virtual void updateSize(const Game& game);

	virtual bool Visible() const { return visible; }
	virtual void Visible(bool visible_) { visible = visible_; }

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
	virtual void update(Game& game) {}

	virtual bool getProperty(const std::string& prop, Variable& var) const;
	virtual const Queryable* getQueryable(const std::string& prop) const { return nullptr; }
};
