#pragma once

#include "Anchor.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "Variable.h"

class Game;
class Level;

class LevelObject : public sf::Drawable
{
public:
	// Move
	virtual const sf::Vector2f& Position() const = 0;
	virtual void Position(const sf::Vector2f& pos) = 0;
	virtual sf::Vector2f Size() const = 0;
	virtual void Size(const sf::Vector2f& size) = 0;

	// Map Position
	virtual const sf::Vector2i& MapPosition() const = 0;
	virtual void MapPosition(const sf::Vector2i& pos) = 0;

	// Update
	virtual void update(Game& game, Level& level) = 0;

	virtual bool getProperty(const std::string& prop, Variable& var) const = 0;
};
