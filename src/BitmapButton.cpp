#include "BitmapButton.h"
#include "Game.h"
#include "GameUtils.h"

void BitmapButton::updateSize(const Game& game)
{
	if (game.StretchToFit() == true)
	{
		return;
	}
	auto pos = sprite.getPosition();
	auto size = sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldWindowSize(), game.WindowSize());
	sprite.setPosition(pos);
	if (resizable == true)
	{
		sprite.setTextureRect(sf::IntRect(0, 0, (int)size.x, (int)size.y));
	}
}

void BitmapButton::update(Game& game)
{
	if (visible == false)
	{
		return;
	}
	auto contains = sprite.getGlobalBounds().contains(game.MousePositionf());
	Button::updateEvents(game, contains);
}
