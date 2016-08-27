#include "LoadingScreen.h"
#include <cmath>
#include "Game.h"
#include "GameUtils.h"

void LoadingScreen::setProgress(int percent_)
{
	percent = percent_;
	if (percent_ <= 0)
	{
		progressBar.setSize(sf::Vector2f());
	}
	else if (percent_ >= 100)
	{
		progressBar.setSize(barSize);
	}
	else
	{
		sf::Vector2f newSize(barSize);
		newSize.x = std::round(newSize.x * (percent_ / 100.0f));
		progressBar.setSize(newSize);
	}
}

void LoadingScreen::updateSize(const Game& game)
{
	if (game.StretchToFit() == true)
	{
		return;
	}
	auto pos = sprite.getPosition();
	sf::Vector2f size((float)sprite.getTextureRect().width, (float)sprite.getTextureRect().height);
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldWindowSize(), game.WindowSize());
	sprite.setPosition(pos);
	progressBar.setPosition(pos + barPosOffset);
}
