#pragma once

#include "Actions/Action.h"
#include "Anchor.h"
#include <memory>
#include "Queryable.h"
#include <SFML/Graphics.hpp>

class Game;

class UIObject : public virtual sf::Drawable, public virtual Queryable
{
public:
	// Action
	virtual std::shared_ptr<Action> getAction(uint16_t nameHash16) = 0;
	virtual bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) = 0;

	// Anchor
	virtual void setAnchor(const Anchor anchor) = 0;
	virtual void updateSize(const Game& game) = 0;

	// Move
	virtual const sf::Vector2f& DrawPosition() const = 0;
	virtual const sf::Vector2f& Position() const = 0;
	virtual void Position(const sf::Vector2f& pos) = 0;
	virtual sf::Vector2f Size() const = 0;
	virtual void Size(const sf::Vector2f& size) = 0;

	// Update
	virtual void update(Game& game) = 0;

	// Visible
	virtual bool Visible() const = 0;
	virtual void Visible(bool visible) = 0;
};
