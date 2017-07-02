#pragma once

#include <SFML/Graphics.hpp>
#include "UIObject.h"

class Circle : public sf::CircleShape, public UIObject
{
private:
	Anchor anchor{ Anchor::Top | Anchor::Left };
	bool visible{ true };

public:
	Circle(float radius = 0, std::size_t pointCount = 30) : sf::CircleShape(radius, pointCount) {}

	virtual std::shared_ptr<Action> getAction(uint16_t nameHash16) { return nullptr; }
	virtual void setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) {}

	virtual void setAnchor(const Anchor anchor_) { anchor = anchor_; }
	virtual void updateSize(const Game& game);

	virtual const sf::Vector2f& DrawPosition() const { return this->getPosition(); }
	virtual const sf::Vector2f& Position() const { return this->getPosition(); }
	virtual void Position(const sf::Vector2f& position) { this->setPosition(position); }
	virtual sf::Vector2f Size() const { return sf::Vector2f(this->getLocalBounds().width, this->getLocalBounds().height); }
	virtual void Size(const sf::Vector2f& size) {}

	virtual bool Visible() const { return visible; }
	virtual void Visible(bool visible_) { visible = visible_; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (visible == true)
		{
			target.draw(static_cast<sf::CircleShape>(*this), states);
		}
	}

	virtual void update(Game& game) {}

	virtual bool getProperty(const std::string& prop, Variable& var) const;
	virtual const Queryable* getQueryable(const std::string& prop) const { return nullptr; }
};
