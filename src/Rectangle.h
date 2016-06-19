#pragma once

#include <SFML/Graphics.hpp>
#include "UIObject.h"

class Rectangle : public sf::RectangleShape, public UIObject
{
private:
	Anchor anchor{ Anchor::Top | Anchor::Left };
	bool visible{ true };

public:
	Rectangle(const sf::Vector2f& size = sf::Vector2f(0, 0)) : sf::RectangleShape(size) {}

	virtual void setAnchor(const Anchor anchor_) { anchor = anchor_; }
	virtual void updateSize(const Game& game);

	virtual const sf::Vector2f& DrawPosition() const { return this->getPosition(); }
	virtual const sf::Vector2f& Position() const { return this->getPosition(); }
	virtual void Position(const sf::Vector2f& position) { this->setPosition(position); }
	virtual sf::Vector2f Size() const { return this->getSize(); }
	virtual void Size(const sf::Vector2f& size) { this->setSize(size); }

	virtual bool Visible() const { return visible; }
	virtual void Visible(bool visible_) { visible = visible_; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (visible == true)
		{
			target.draw(static_cast<sf::RectangleShape>(*this), states);
		}
	}

	virtual void update(Game& game) {}

	virtual Variable getProperty(const std::string& prop) const;
};
