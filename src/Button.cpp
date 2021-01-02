#include "Button.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils/Utils.h"

std::shared_ptr<Action> Button::getAction(uint16_t nameHash16) const noexcept
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

bool Button::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept
{
	switch (nameHash16)
	{
	case str2int16("click"):
		clickAction = action;
		break;
	case str2int16("rightClick"):
		rightClickAction = action;
		break;
	case str2int16("doubleClick"):
		doubleClickAction = action;
		break;
	case str2int16("clickDrag"):
		clickDragAction = action;
		break;
	case str2int16("clickIn"):
		clickInAction = action;
		break;
	case str2int16("clickOut"):
		clickOutAction = action;
		break;
	case str2int16("focus"):
		focusAction = action;
		break;
	case str2int16("hoverEnter"):
		hoverEnterAction = action;
		break;
	case str2int16("hoverLeave"):
		hoverLeaveAction = action;
		break;
	default:
		return false;
	}
	return true;
}

bool Button::click(Game& game, bool playSound, const std::shared_ptr<Action>& action)
{
	if (enabled == true)
	{
		if (action != nullptr)
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
				game.addPlayingSound(clickSound);
			}
			game.Events().addBack(action);
			return true;
		}
	}
	return false;
}

bool Button::click(Game& game, bool playSound)
{
	return click(game, playSound, clickAction);
}

bool Button::rightClick(Game& game, bool playSound)
{
	return click(game, playSound, rightClickAction);
}

void Button::focus(Game& game) const
{
	game.addPlayingSound(focusSound);
	game.Events().addFront(focusAction);
}

void Button::onHover(Game& game, bool contains)
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

void Button::onMouseButtonPressed(Game& game, bool contains)
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
			if (rightClick(game, true) == true)
			{
				game.clearMousePressed();
			}
		}
	}
	break;
	default:
		break;
	}
}

void Button::onMouseButtonReleased(Game& game, bool contains)
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
			if (rightClick(game, true) == true)
			{
				game.clearMouseReleased();
			}
		}
	}
	break;
	default:
		break;
	}
}

void Button::onMouseMoved(Game& game)
{
	if (beingDragged == true)
	{
		if (clickDragAction != nullptr)
		{
			game.Events().addBack(clickDragAction);
		}
	}
}

void Button::onTouchBegan(Game& game, bool contains)
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

void Button::onTouchEnded(Game& game, bool contains)
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
			if (rightClick(game, true) == true)
			{
				game.clearTouchEnded();
			}
		}
	}
	break;
	default:
		break;
	}
}

void Button::updateEvents(Game& game, bool contains)
{
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
		onHover(game, contains);
		onMouseMoved(game);
	}
	if (game.hasTouchBegan() == true)
	{
		onTouchBegan(game, contains);
	}
	if (game.hasTouchMoved() == true)
	{
		onHover(game, contains);
		onMouseMoved(game);
	}
	if (game.hasTouchEnded() == true)
	{
		onTouchEnded(game, contains);
	}
	if (contains == true &&
		captureInputEvents != InputEventType::None)
	{
		game.clearInputEvents(captureInputEvents);
	}
}

bool Button::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	switch (propHash)
	{
	case str2int16("clickUp"):
		var = Variable(clickUp);
		break;
	case str2int16("enabled"):
		var = Variable(enabled);
		break;
	default:
		return false;
	}
	return true;
}
