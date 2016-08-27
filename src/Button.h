#pragma once

#include "Alignment.h"
#include <memory>
#include "SFML/Audio/SoundBuffer.hpp"
#include "UIObject.h"

class Button : public UIObject
{
public:
	virtual void click(Game& game, bool playSound) = 0;
	virtual void enable(bool enable) = 0;
	virtual void focus(Game& game) = 0;
	virtual void focusEnabled(bool focusOnClick) = 0;
	virtual bool isEnabled() = 0;
	virtual void setClickAction(const std::shared_ptr<Action>& action) = 0;
	virtual void setClickDragAction(const std::shared_ptr<Action>& action) = 0;
	virtual void setClickInAction(const std::shared_ptr<Action>& action) = 0;
	virtual void setClickOutAction(const std::shared_ptr<Action>& action) = 0;
	virtual void setClickSound(const std::shared_ptr<sf::SoundBuffer>& buffer) = 0;
	virtual void setClickUp(bool clickUp) = 0;
	virtual void setColor(const sf::Color& color) = 0;
	virtual void setDoubleClickAction(const std::shared_ptr<Action>& action) = 0;
	virtual void setFocusAction(const std::shared_ptr<Action>& action) = 0;
	virtual void setFocusSound(const std::shared_ptr<sf::SoundBuffer>& buffer) = 0;
	virtual void setHoverEnterAction(const std::shared_ptr<Action>& action) = 0;
	virtual void setHoverLeaveAction(const std::shared_ptr<Action>& action) = 0;
	virtual void setToggle(bool toggle) = 0;
	virtual void setToggleAction(const std::shared_ptr<Action>& action) = 0;
};
