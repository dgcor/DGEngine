#include "BitmapButton.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

std::shared_ptr<Action> BitmapButton::getAction(uint16_t nameHash16)
{
	switch (nameHash16)
	{
	case str2int16("click"):
		return clickAction;
	case str2int16("rightClick"):
		return rightClickAction;
	case str2int16("doubleClick"):
		return doubleClickAction;
	case str2int16("clickDrag"):
		return clickDragAction;
	case str2int16("clickIn"):
		return clickInAction;
	case str2int16("clickOut"):
		return clickOutAction;
	case str2int16("focus"):
		return focusAction;
	case str2int16("hoverEnter"):
		return hoverEnterAction;
	case str2int16("hoverLeave"):
		return hoverLeaveAction;
	default:
		return nullptr;
	}
}

void BitmapButton::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action)
{
	switch (nameHash16)
	{
	case str2int16("click"):
		clickAction = action;
		return;
	case str2int16("rightClick"):
		rightClickAction = action;
		return;
	case str2int16("doubleClick"):
		doubleClickAction = action;
		return;
	case str2int16("clickDrag"):
		clickDragAction = action;
		return;
	case str2int16("clickIn"):
		clickInAction = action;
		return;
	case str2int16("clickOut"):
		clickOutAction = action;
		return;
	case str2int16("focus"):
		focusAction = action;
		return;
	case str2int16("hoverEnter"):
		hoverEnterAction = action;
		return;
	case str2int16("hoverLeave"):
		hoverLeaveAction = action;
		return;
	}
}

bool BitmapButton::click(Game& game, bool playSound)
{
	if (enabled == true)
	{
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
			game.Events().addBack(clickAction);
			return true;
		}
	}
	return false;
}

void BitmapButton::focus(Game& game) const
{
	game.addPlayingSound(focusSound.get());
	game.Events().addFront(focusAction);
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
	auto size = sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldWindowSize(), game.WindowSize());
	sprite.setPosition(pos);
	if (resizable == true)
	{
		sprite.setTextureRect(sf::IntRect(0, 0, (int)size.x, (int)size.y));
	}
}

void BitmapButton::onHover(Game& game, bool contains)
{
	if (contains == true)
	{
		if (hovered == false)
		{
			hovered = true;
			if (hoverEnterAction != nullptr)
			{
				game.Events().addBack(hoverEnterAction);
			}
		}
	}
	else
	{
		if (hovered == true)
		{
			hovered = false;
			if (hoverLeaveAction != nullptr)
			{
				game.Events().addFront(hoverLeaveAction);
			}
		}
	}
}

void BitmapButton::onMouseButtonPressed(Game& game, bool contains)
{
	if (contains == false)
	{
		return;
	}
	switch (game.MousePress().button)
	{
	case sf::Mouse::Left:
	{
		beingDragged = true;
		wasLeftClicked = true;

		if (clickInAction != nullptr)
		{
			game.Events().addBack(clickInAction);
		}
		if (clickUp == false)
		{
			if (doubleClickAction != nullptr &&
				mouseDblClickClock.restart().asMilliseconds() <= GameUtils::DoubleClickDelay)
			{
				game.clearMousePressed();
				game.Events().addBack(doubleClickAction);
			}
			else
			{
				if (click(game, true) == true)
				{
					game.clearMousePressed();
				}
			}
		}
	}
	break;
	case sf::Mouse::Right:
	{
		wasRightClicked = true;
		if (clickUp == false)
		{
			if (rightClickAction != nullptr)
			{
				game.clearMousePressed();
				game.Events().addBack(rightClickAction);
			}
		}
	}
	break;
	default:
		break;
	}
}

void BitmapButton::onMouseButtonReleased(Game& game, bool contains)
{
	switch (game.MouseRelease().button)
	{
	case sf::Mouse::Left:
	{
		if (wasLeftClicked == false)
		{
			break;
		}
		wasLeftClicked = false;
		if (clickUp == true &&
			contains == true)
		{
			if (click(game, true) == true)
			{
				game.clearMouseReleased();
			}
		}
		beingDragged = false;
		if (clickOutAction != nullptr)
		{
			game.Events().addBack(clickOutAction);
		}
	}
	break;
	case sf::Mouse::Right:
	{
		if (wasRightClicked == false)
		{
			break;
		}
		wasRightClicked = false;
		if (clickUp == true &&
			contains == true)
		{
			if (rightClickAction != nullptr)
			{
				game.clearMouseReleased();
				game.Events().addBack(rightClickAction);
			}
		}
	}
	break;
	default:
		break;
	}
}

void BitmapButton::onMouseMoved(Game& game)
{
	if (beingDragged == true)
	{
		if (clickDragAction != nullptr)
		{
			game.Events().addBack(clickDragAction);
		}
	}
}

void BitmapButton::onTouchBegan(Game& game, bool contains)
{
	if (contains == false)
	{
		return;
	}
	switch (game.TouchBegan().finger)
	{
	case 0:
	{
		beingDragged = true;
		wasLeftClicked = true;

		if (clickInAction != nullptr)
		{
			game.Events().addBack(clickInAction);
		}
	}
	break;
	case 1:
	{
		wasRightClicked = true;
	}
	break;
	default:
		break;
	}
}

void BitmapButton::onTouchEnded(Game& game, bool contains)
{
	switch (game.TouchEnded().finger)
	{
	case 0:
	{
		if (wasLeftClicked == false)
		{
			break;
		}
		wasLeftClicked = false;
		if (contains == true)
		{
			if (doubleClickAction != nullptr &&
				mouseDblClickClock.restart().asMilliseconds() <= GameUtils::DoubleClickDelay)
			{
				game.clearTouchEnded();
				game.Events().addBack(doubleClickAction);
			}
			else
			{
				if (click(game, true) == true)
				{
					game.clearTouchEnded();
				}
			}
		}
		beingDragged = false;
		if (clickOutAction != nullptr)
		{
			game.Events().addBack(clickOutAction);
		}
	}
	break;
	case 1:
	{
		if (wasRightClicked == false)
		{
			break;
		}
		wasRightClicked = false;
		if (contains == true)
		{
			if (rightClickAction != nullptr)
			{
				game.clearTouchEnded();
				game.Events().addBack(rightClickAction);
			}
		}
	}
	break;
	default:
		break;
	}
}

void BitmapButton::update(Game& game)
{
	if (visible == false)
	{
		return;
	}

	auto contains = sprite.getGlobalBounds().contains(game.MousePositionf());

	onHover(game, contains);

	if (game.wasMousePressed() == true)
	{
		onMouseButtonPressed(game, contains);
	}
	if (game.wasMouseReleased() == true)
	{
		onMouseButtonReleased(game, contains);
	}
	if (game.wasMouseMoved() == true)
	{
		onMouseMoved(game);
	}
	if (game.hasTouchBegan() == true)
	{
		onTouchBegan(game, contains);
	}
	if (game.hasTouchMoved() == true)
	{
		onMouseMoved(game);
	}
	if (game.hasTouchEnded() == true)
	{
		onTouchEnded(game, contains);
	}
	if (contains == true &&
		captureInputEvents == true)
	{
		game.clearInputEvents();
	}
}

bool BitmapButton::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	return GameUtils::getUIObjProp(*this, str2int16(props.first.c_str()), props.second, var);
}
