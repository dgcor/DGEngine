#pragma once

#include "SFML/VertexShape.h"
#include "UIObject.h"

class Shape : public VertexShape, public UIObject
{
private:
	Anchor anchor{ Anchor::Top | Anchor::Left };
	bool visible{ true };

public:
	explicit Shape(sf::PrimitiveType type) : VertexShape(type) {}

	Anchor getAnchor() const noexcept override { return anchor; }
	void setAnchor(const Anchor anchor_) noexcept override { anchor = anchor_; }
	void updateSize(const Game& game) override;

	const sf::Vector2f& DrawPosition() const override { return getPosition(); }
	const sf::Vector2f& Position() const override { return getPosition(); }
	void Position(const sf::Vector2f& position) override { setPosition(position); }
	sf::Vector2f Size() const override { return sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height); }
	void Size(const sf::Vector2f& size) override;

	bool Visible() const noexcept override { return visible; }
	void Visible(bool visible_) noexcept override { visible = visible_; }

	void draw(const Game& game, sf::RenderTarget& target) const override
	{
		if (visible == true)
		{
			target.draw(static_cast<VertexShape>(*this));
		}
	}

	bool getProperty(const std::string_view prop, Variable& var) const override;
};
