#pragma once

#include "Actions/Action.h"
#include "MapCoord.h"
#include <memory>
#include "Number.h"
#include "Palette.h"
#include "Queryable.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include "Variable.h"

class Game;
class Level;

class LevelObject : public sf::Drawable, public Queryable
{
public:
	// Move
	virtual const sf::Vector2f& Position() const = 0;
	virtual sf::Vector2f Size() const = 0;

	// Map Position
	virtual const MapCoord& MapPosition() const = 0;
	virtual void MapPosition(const MapCoord& pos) = 0;

	// Game
	virtual void executeAction(Game& game) const = 0;
	virtual bool getNumberProp(const char* prop, Number32& value) const = 0;
	virtual bool hasPalette() const = 0;
	virtual bool Hoverable() const = 0;
	virtual void Hoverable(bool hoverable) = 0;
	virtual bool Passable() const = 0;
	virtual void setAction(const std::shared_ptr<Action>& action) = 0;
	virtual void setColor(const sf::Color& color) = 0;
	virtual void setOutline(const sf::Color& outline, const sf::Color& ignore) = 0;
	virtual void setOutlineOnHover(bool outlineOnHover) = 0;
	virtual void setPalette(const std::shared_ptr<Palette>& palette) = 0;

	// Update
	virtual void update(Game& game, Level& level) = 0;

	virtual void setProperty(const std::string& prop, const Variable& val) = 0;
};
