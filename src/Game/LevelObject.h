#pragma once

#include "Actions/Action.h"
#include "Anchor.h"
#include "MapCoord.h"
#include <memory>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
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
	virtual const MapCoord& MapPosition() const = 0;
	virtual void MapPosition(const MapCoord& pos) = 0;

	// Game
	virtual void executeAction(Game& game) const = 0;
	virtual bool Passable() const = 0;
	virtual void setAction(const std::shared_ptr<Action>& action) = 0;

	// Update
	virtual void update(Game& game, Level& level) = 0;

	virtual bool getProperty(const std::string& prop, Variable& var) const = 0;
	virtual void setProperty(const std::string& prop, const Variable& val) = 0;
};
