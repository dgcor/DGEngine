#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include "UIObject.h"

class Rectangle : public sf::RectangleShape, public UIObject
{
private:
	Anchor anchor{ Anchor::Top | Anchor::Left };
	bool visible{ true };

public:
	Rectangle(const sf::Vector2f& size = sf::Vector2f(0, 0)) : sf::RectangleShape(size) {}

	virtual Anchor getAnchor() const noexcept { return anchor; }
	virtual void setAnchor(const Anchor anchor_) noexcept { anchor = anchor_; }
	virtual void updateSize(const Game& game);

	virtual const sf::Vector2f& DrawPosition() const { return this->getPosition(); }
	virtual const sf::Vector2f& Position() const { return this->getPosition(); }
	virtual void Position(const sf::Vector2f& position) { this->setPosition(position); }
	virtual sf::Vector2f Size() const { return this->getSize(); }
	virtual void Size(const sf::Vector2f& size) { this->setSize(size); }

	virtual bool Visible() const noexcept { return visible; }
	virtual void Visible(bool visible_) noexcept { visible = visible_; }

	virtual void draw(const Game& game, sf::RenderTarget& target) const
	{
		if (visible == true)
		{
			target.draw(static_cast<sf::RectangleShape>(*this));
		}
	}

	virtual bool getProperty(const std::string_view prop, Variable& var) const;
};
