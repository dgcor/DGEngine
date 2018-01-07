#include "StringButton.h"
#include "Game.h"

std::shared_ptr<Action> StringButton::getAction(uint16_t nameHash16)
{
	auto action = Button::getAction(nameHash16);
	if (action == nullptr)
	{
		action = Text2::getAction(nameHash16);
	}
	return action;
}

bool StringButton::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action)
{
	if (Button::setAction(nameHash16, action) == false)
	{
		return Text2::setAction(nameHash16, action);
	}
	return true;
}

void StringButton::update(Game& game)
{
	if (Text2::Visible() == false)
	{
		return;
	}
	Text2::update(game);
	auto contains = Text2::getGlobalBounds().contains(game.MousePositionf());
	Button::update(game, contains);
}
