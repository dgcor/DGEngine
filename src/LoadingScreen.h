#pragma once

#include "Anchor.h"
#include "Actions/Action.h"
#include <memory>
#include <SFML/Graphics.hpp>

class LoadingScreen
{
private:
	sf::Sprite sprite;
	sf::RectangleShape progressBar;
	sf::Vector2f barSize;
	sf::Vector2f barPosOffset;
	Anchor anchor{ Anchor::Top | Anchor::Left };
	std::shared_ptr<Action> action;
	int percent{ 0 };

public:
	LoadingScreen() {}
	LoadingScreen(const sf::Texture& tex) : sprite(tex) {}

	void setPosition(const sf::Vector2f& position)
	{
		sprite.setPosition(position);
		progressBar.setPosition(position + barPosOffset);
	}
	sf::Vector2f getSize() const
	{
		return sf::Vector2f((float)sprite.getTextureRect().width, (float)sprite.getTextureRect().height);
	}
	void setProgressBarColor(const sf::Color& color) { progressBar.setFillColor(color); }
	void setProgressBarPositionOffset(const sf::Vector2f& offset)
	{
		barPosOffset = offset;
		progressBar.setPosition(sprite.getPosition() + offset);
	}
	void setProgressBarSize(const sf::Vector2f& size) noexcept { barSize = size; }
	int getProgress() const noexcept { return percent; }
	void setProgress(int percent_);
	bool isComplete() const noexcept { return percent >= 100; }

	const sf::IntRect& getTextureRect() const { return sprite.getTextureRect(); }
	void setTextureRect(const sf::IntRect& rectangle) { sprite.setTextureRect(rectangle); }

	const std::shared_ptr<Action>& getAction() const noexcept { return action; }
	void setAction(const std::shared_ptr<Action>& action_) noexcept { action = action_; }

	Anchor getAnchor() const noexcept { return anchor; }
	void setAnchor(const Anchor anchor_) noexcept { anchor = anchor_; }

	void updateSize(const Game& game);

	void draw(sf::RenderTarget& target) const
	{
		target.draw(sprite);
		target.draw(progressBar);
	}
};
