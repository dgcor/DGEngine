#include "BitmapButton.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

void BitmapButton::click(Game& game, bool playSound)
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

void BitmapButton::focus(Game& game)
{
	game.addPlayingSound(focusSound.get());
	game.Events().addBack(focusAction);
}

void BitmapButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (visible == true)
	{
		target.draw(sprite, states);
	}
}

void BitmapButton::updateSize(const Game& game)
{
	if (game.StretchToFit() == true)
	{
		return;
	}
	auto pos = sprite.getPosition();
	auto size = sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);;
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

	auto rect = sprite.getGlobalBounds();
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

bool BitmapButton::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.size() > 1)
	{
		auto props = Utils::splitString(prop, '.');
		if (props.size() > 0)
		{
			return GameUtils::getUIObjProp(*this, Utils::str2int(props[0].c_str()), props, var);
		}
	}
	return false;
}
