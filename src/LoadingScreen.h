#pragma once

#include "Anchor.h"
#include "Actions/Action.h"
#include <memory>
#include <SFML/Graphics.hpp>

class LoadingScreen : public sf::Drawable
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
	LoadingScreen(const sf::Texture& tex) : sprite(tex) {}

	void setPosition(const sf::Vector2f& position)
	{
		sprite.setPosition(position);
		progressBar.setPosition(position + barPosOffset);
	}
	void setProgressBarColor(const sf::Color& color) { progressBar.setFillColor(color); }
	void setProgressBarPositionOffset(const sf::Vector2f& offset)
	{
		barPosOffset = offset;
		progressBar.setPosition(sprite.getPosition() + offset);
	}
	void setProgressBarSize(const sf::Vector2f& size) { barSize = size; }
	void setProgress(int percent_);
	bool isComplete() const { return percent >= 100; }

	const std::shared_ptr<Action>& getAction() const { return action; }
	void setAction(const std::shared_ptr<Action>& action_) { action = action_; }

	void setAnchor(const Anchor anchor_) { anchor = anchor_; }

	void updateSize(const Game& game);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(sprite, states);
		target.draw(progressBar, states);
	}
};
