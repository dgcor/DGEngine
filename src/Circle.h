#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include "UIObject.h"

class Circle : public sf::CircleShape, public UIObject
{
private:
	Anchor anchor{ Anchor::Top | Anchor::Left };
	bool visible{ true };

public:
	Circle(float radius = 0, std::size_t pointCount = 30) : sf::CircleShape(radius, pointCount) {}

	virtual Anchor getAnchor() const noexcept { return anchor; }
	virtual void setAnchor(const Anchor anchor_) noexcept { anchor = anchor_; }
	virtual void updateSize(const Game& game);

	virtual const sf::Vector2f& DrawPosition() const { return this->getPosition(); }
	virtual const sf::Vector2f& Position() const { return this->getPosition(); }
	virtual void Position(const sf::Vector2f& position) { this->setPosition(position); }
	virtual sf::Vector2f Size() const { return sf::Vector2f(this->getLocalBounds().width, this->getLocalBounds().height); }
	virtual void Size(const sf::Vector2f& size) noexcept {}

	virtual bool Visible() const noexcept { return visible; }
	virtual void Visible(bool visible_) noexcept { visible = visible_; }

	virtual void draw(const Game& game, sf::RenderTarget& target) const
	{
		if (visible == true)
		{
			target.draw(static_cast<sf::CircleShape>(*this));
		}
	}

	virtual bool getProperty(const std::string_view prop, Variable& var) const;
};
