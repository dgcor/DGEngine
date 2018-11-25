#pragma once

#include "MapCoord.h"
#include <memory>
#include "Number.h"
#include "Palette.h"
#include "Queryable.h"
#include "Save/SaveProperties.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
#include <string_view>
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
	virtual bool getNumberProp(const std::string_view prop, Number32& value) const = 0;
	virtual bool hasPalette() const = 0;
	virtual bool Hoverable() const = 0;
	virtual void Hoverable(bool hoverable) = 0;
	virtual bool Passable() const = 0;
	virtual void setColor(const sf::Color& color) = 0;
	virtual void setOutline(const sf::Color& outline, const sf::Color& ignore) = 0;
	virtual void setOutlineOnHover(bool outlineOnHover) = 0;
	virtual void setPalette(const std::shared_ptr<Palette>& palette) = 0;

	virtual std::string_view getId() const = 0;
	virtual std::string_view getClassId() const = 0;

	// serialize this object.
	// serializeObj - currently is a RapidJson writer class
	virtual void serialize(void* serializeObj, Save::Properties& props,
		const Game& game, const Level& level) const = 0;

	// Update
	virtual void update(Game& game, Level& level) = 0;

	virtual void setProperty(const std::string_view prop, const Variable& val) = 0;
};
