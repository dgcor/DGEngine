#include "StringButton.h"
#include "Game.h"

std::shared_ptr<Action> StringButton::getAction(uint16_t nameHash16) const noexcept
{
	auto action = Button::getAction(nameHash16);
	if (action == nullptr)
	{
		action = Text::getAction(nameHash16);
	}
	return action;
}

bool StringButton::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept
{
	if (Button::setAction(nameHash16, action) == false)
	{
		return Text::setAction(nameHash16, action);
	}
	return true;
}

void StringButton::update(Game& game)
{
	if (Text::Visible() == false)
	{
		return;
	}
	Text::update(game);
	auto contains = Text::getGlobalBounds().contains(game.MousePositionf());
	Button::updateEvents(game, contains);
}
