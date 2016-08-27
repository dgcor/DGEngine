#pragma once

#include "Anchor.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "Variable.h"

class Game;

class UIObject : public sf::Drawable
{
public:
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

	virtual bool getProperty(const std::string& prop, Variable& var) const = 0;
};
