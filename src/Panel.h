#pragma once

#include "UIObject.h"
#include <vector>

class Panel : public UIObject
{
private:
	std::vector<std::weak_ptr<UIObject>> drawables;
	sf::Vector2f position;
	Anchor anchor{ Anchor::Top | Anchor::Left };
	bool visible{ true };
	bool relativePositions{ true };

public:
	Panel(bool relativePositions_) : relativePositions(relativePositions_) {}

	void addDrawable(const std::shared_ptr<UIObject>& obj);
	UIObject* getDrawable(size_t idx) const;
	size_t getItemCount() const noexcept { return drawables.size(); }

	virtual Anchor getAnchor() const noexcept { return anchor; }
	virtual void setAnchor(const Anchor anchor_) noexcept { anchor = anchor_; }
	virtual void updateSize(const Game& game);

	// calculate the draw position/size based on all its children's draw positions/sizes
	void getDrawPositionAndSize(sf::Vector2f& drawPos, sf::Vector2f& drawSize) const;

	virtual const sf::Vector2f& DrawPosition() const { return position; }
	virtual const sf::Vector2f& Position() const { return position; }
	virtual void Position(const sf::Vector2f& newPosition);
	virtual sf::Vector2f Size() const { return {}; }
	virtual void Size(const sf::Vector2f& size) {}

	virtual bool Visible() const noexcept { return visible; }
	virtual void Visible(bool visible_) noexcept { visible = visible_; }

	void draw(const Game& game, sf::RenderTarget& target,
		const sf::FloatRect& visibleRect) const;
	virtual void draw(const Game& game, sf::RenderTarget& target) const;
	virtual void update(Game& game);

	virtual bool getProperty(const std::string_view prop, Variable& var) const;
};
