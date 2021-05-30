#include "BitmapButton.h"
#include "Game.h"

void BitmapButton::update(Game& game)
{
	if (visible == false)
	{
		return;
	}
	auto contains = sprite.getGlobalBounds().contains(game.MousePositionf());
	Button::updateEvents(game, contains);
}

bool BitmapButton::getProperty(const std::string_view prop, Variable& var) const
{
	if (Button::getProperty(prop, var) == true)
	{
		return true;
	}
	return Image::getProperty(prop, var);
}
