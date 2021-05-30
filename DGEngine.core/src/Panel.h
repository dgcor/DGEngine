#pragma once

#include "UIObject.h"
#include <vector>

class Panel : public UIObject
{
private:
	std::vector<std::weak_ptr<UIObject>> drawables;
	sf::Vector2f position;
	mutable sf::Vector2f drawPosition;
	mutable sf::Vector2f drawSize;
	Anchor anchor{ Anchor::Top | Anchor::Left };
	bool visible{ true };
	bool relativePositions{ true };
	mutable bool sizePosNeedsUpdate{ true };

	// calculate the draw position/size based on all its children's draw positions/sizes
	void updateDrawPositionAndSize() const;

public:
	Panel(bool relativePositions_) : relativePositions(relativePositions_) {}

	void addDrawable(const std::shared_ptr<UIObject>& obj);
	UIObject* getDrawable(size_t idx) const;
	size_t getDrawableCount() const noexcept { return drawables.size(); }

	Anchor getAnchor() const noexcept override { return anchor; }
	void setAnchor(const Anchor anchor_) noexcept override { anchor = anchor_; }
	void updateSize(const Game& game) override;

	const sf::Vector2f& DrawPosition() const override { return drawPosition; }
	const sf::Vector2f& Position() const override;
	void Position(const sf::Vector2f& newPosition) override;
	sf::Vector2f Size() const override;
	void Size(const sf::Vector2f& size) override {}

	bool Visible() const noexcept override { return visible; }
	void Visible(bool visible_) noexcept override { visible = visible_; }

	bool draw(const Game& game, sf::RenderTarget& target,
		const sf::FloatRect& visibleRect) const;
	void draw(const Game& game, sf::RenderTarget& target) const override;
	void update(Game& game) override;

	bool getProperty(const std::string_view prop, Variable& var) const override;
};
