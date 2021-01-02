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

	Anchor getAnchor() const noexcept override { return anchor; }
	void setAnchor(const Anchor anchor_) noexcept override { anchor = anchor_; }
	void updateSize(const Game& game) override;

	const sf::Vector2f& DrawPosition() const override { return getPosition(); }
	const sf::Vector2f& Position() const override { return getPosition(); }
	void Position(const sf::Vector2f& position) override { setPosition(position); }
	sf::Vector2f Size() const override { return sf::Vector2f(getLocalBounds().width, getLocalBounds().height); }
	void Size(const sf::Vector2f& size) noexcept override {}

	bool Visible() const noexcept override { return visible; }
	void Visible(bool visible_) noexcept override { visible = visible_; }

	void draw(const Game& game, sf::RenderTarget& target) const override
	{
		if (visible == true)
		{
			target.draw(static_cast<sf::CircleShape>(*this));
		}
	}

	bool getProperty(const std::string_view prop, Variable& var) const override;
};
