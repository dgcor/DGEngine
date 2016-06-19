#include "StringButton.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

using Utils::str2int;

void StringButton::click(Game& game, bool playSound)
{
	if (enabled == false)
	{
		return;
	}
	if (clickAction != nullptr)
	{
		if (focusEnable == true)
		{
			game.Resources().setFocused(this);
			if (focusOnClick == true)
			{
				game.Events().addBack(focusAction);
			}
		}
		if (playSound == true)
		{
			game.addPlayingSound(clickSound.get());
		}
		if (toggleAction == nullptr)
		{
			game.Events().addBack(clickAction);
		}
		else
		{
			if (toggled == false)
			{
				toggled = true;
				game.Events().addBack(clickAction);
			}
			else
			{
				toggled = false;
				game.Events().addBack(toggleAction);
			}
		}
	}
}

void StringButton::focus(Game& game)
{
	game.addPlayingSound(focusSound.get());
	game.Events().addBack(focusAction);
}

void StringButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (visible == true)
	{
		target.draw(*text, states);
	}
}

void StringButton::update(Game& game)
{
	if (visible == false)
	{
		return;
	}

	text->update(game);

	auto rect = text->getGlobalBounds();
	if (rect.contains(game.MousePosition()))
	{
		if (hovered == false)
		{
			hovered = true;
			if (hoverEnterAction != nullptr)
			{
				game.Events().addBack(hoverEnterAction);
			}
		}
		if (game.getMouseButton() == sf::Mouse::Left)
		{
			if (game.wasMouseClicked() == true)
			{
				game.clearMouseClicked();
				beingDragged = true;
				if (clickInAction != nullptr)
				{
					game.Events().addFront(clickInAction);
				}
				if (clickUp == false)
				{
					click(game, true);
					if (game.wasMouseDoubleClicked() == true)
					{
						if (doubleClickAction != nullptr)
						{
							game.Events().addBack(doubleClickAction);
						}
					}
				}
			}
			else if (game.wasMouseReleased() == true)
			{
				if (clickOutAction != nullptr)
				{
					game.Events().addFront(clickOutAction);
				}
				if (clickUp == true && beingDragged == true)
				{
					click(game, true);
				}
				beingDragged = false;
			}
		}
	}
	else
	{
		if (game.wasMouseReleased() == true && game.getMouseButton() == sf::Mouse::Left)
		{
			beingDragged = false;
			if (clickOutAction != nullptr)
			{
				game.Events().addFront(clickOutAction);
			}
		}
		if (hovered == true)
		{
			hovered = false;
			if (hoverLeaveAction != nullptr)
			{
				game.Events().addFront(hoverLeaveAction);
			}
		}
	}
	if (beingDragged == true && game.wasMouseMoved() == true)
	{
		if (clickDragAction != nullptr)
		{
			game.Events().addFront(clickDragAction);
		}
	}
}
