#pragma once

#include "Game/Anchor.h"
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

	auto getAnchor() const noexcept { return anchor; }
	void setAnchor(const Anchor anchor_) noexcept { anchor = anchor_; }

	auto& getCenter() const { return view.getCenter(); }
	void setCenter(float x, float y) { view.setCenter(x, y); }
	void setCenter(const sf::Vector2f& center) { view.setCenter(center); }

	sf::Vector2f getPosition(const sf::Vector2f& point) const;
	sf::Vector2f getDrawPosition(const sf::Vector2f& point) const;

	auto& getPosition() const noexcept { return position; }
	void setPosition(const sf::Vector2f& position_) noexcept { position = position_; }

	auto& getVisibleSize() const noexcept { return view.getSize(); }
	auto& getSize() const noexcept { return sizeNoZoom; }
	auto& getRoundedSize() const noexcept { return roundedSizeNoZoom; }
	void setSize(float width, float height);
	void setSize(const sf::Vector2f& size_);

	auto& getView() const noexcept { return view; }

	float getZoom() const noexcept { return zoomFactor; }

	void setZoom(float factor) noexcept { zoomFactor = factor; }

	void reset(const sf::FloatRect& rectangle) { view.reset(rectangle); }
	void reset();
};
