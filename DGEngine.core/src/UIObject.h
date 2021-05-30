#pragma once

#include "Actions/Action.h"
#include "Anchor.h"
#include <memory>
#include "Queryable.h"
#include <SFML/Graphics/RenderTarget.hpp>

class Game;

class UIObject : public virtual Queryable
{
protected:
	// gets common getProperty properties.
	bool getUIObjProp(const uint16_t propHash,
		const std::string_view prop, Variable& var) const;

public:
	// Action
	virtual std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept { return nullptr; }
	virtual bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept { return false; }

	// Anchor
	virtual Anchor getAnchor() const = 0;
	virtual void setAnchor(const Anchor anchor) = 0;
	virtual void updateSize(const Game& game) = 0;

	// Move
	virtual const sf::Vector2f& DrawPosition() const = 0;
	virtual const sf::Vector2f& Position() const = 0;
	virtual void Position(const sf::Vector2f& pos) = 0;
	virtual sf::Vector2f Size() const = 0;
	virtual void Size(const sf::Vector2f& size) = 0;

	// Draw and Update
	virtual void draw(const Game& game, sf::RenderTarget& target) const = 0;
	virtual void update(Game& game) {}

	// Visible
	virtual bool Visible() const = 0;
	virtual void Visible(bool visible) = 0;
};
