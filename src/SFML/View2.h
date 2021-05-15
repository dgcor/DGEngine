#pragma once

#include "Anchor.h"
#include <SFML/Graphics/View.hpp>

class Game;

class View2
{
private:
	sf::View view;
	sf::Vector2f position;
	sf::Vector2f sizeNoZoom;
	sf::Vector2f roundedSizeNoZoom;
	float zoomFactor{ 1.f };
	Anchor anchor{ Anchor::Top | Anchor::Left };
	bool forceEvenSize{ false };

	void setViewport(const Game& game);

public:
	View2(bool forceEvenSize_ = false) : forceEvenSize(forceEvenSize_) {}

	void updateSize(const Game& game, bool dontUpdateViewport = false);

	// updates size, zoom and the viewport
	void update(const Game& game, bool dontUpdateViewport = false);

	void setViewport(const sf::FloatRect& viewport) { view.setViewport(viewport); }

	Anchor getAnchor() const noexcept { return anchor; }
	void setAnchor(const Anchor anchor_) noexcept { anchor = anchor_; }

	const sf::Vector2f& getCenter() const { return view.getCenter(); }
	void setCenter(float x, float y) { view.setCenter(x, y); }
	void setCenter(const sf::Vector2f& center) { view.setCenter(center); }

	sf::Vector2f getPosition(const sf::Vector2f& point) const;
	sf::Vector2f getDrawPosition(const sf::Vector2f& point) const;

	const sf::Vector2f& getPosition() const noexcept { return position; }
	void setPosition(const sf::Vector2f& position_) noexcept { position = position_; }

	const sf::Vector2f& getVisibleSize() const noexcept { return view.getSize(); }
	const sf::Vector2f& getSize() const noexcept { return sizeNoZoom; }
	const sf::Vector2f& getRoundedSize() const noexcept { return roundedSizeNoZoom; }
	void setSize(float width, float height);
	void setSize(const sf::Vector2f& size_);

	const sf::View& getView() const noexcept { return view; }

	float getZoom() const noexcept { return zoomFactor; }

	void setZoom(float factor) noexcept { zoomFactor = factor; }

	void reset(const sf::FloatRect& rectangle) { view.reset(rectangle); }
	void reset();
};
