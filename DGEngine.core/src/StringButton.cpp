#include "StringButton.h"
#include "Game.h"

std::shared_ptr<Action> StringButton::getAction(uint16_t nameHash16) const noexcept
{
	auto action = Button::getAction(nameHash16);
	if (action == nullptr)
	{
		action = BindableText::getAction(nameHash16);
	}
	return action;
}

bool StringButton::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept
{
	if (Button::setAction(nameHash16, action) == false)
	{
		return BindableText::setAction(nameHash16, action);
	}
	return true;
}

void StringButton::update(Game& game)
{
	BindableText::update(game);
	if (BindableText::Visible() == false)
	{
		return;
	}
	auto contains = BindableText::getGlobalBounds().contains(game.MousePositionf());
	Button::updateEvents(game, contains);
}

bool StringButton::getProperty(const std::string_view prop, Variable& var) const
{
	if (Button::getProperty(prop, var) == true)
	{
		return true;
	}
	return BindableText::getProperty(prop, var);
}
