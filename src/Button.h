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
	virtual void focus(Game& game) const = 0;
	virtual void focusEnabled(bool focusOnClick) = 0;
	virtual bool isEnabled() const = 0;
	virtual void setClickSound(const std::shared_ptr<sf::SoundBuffer>& buffer) = 0;
	virtual void setClickUp(bool clickUp) = 0;
	virtual void setColor(const sf::Color& color) = 0;
	virtual void setFocusSound(const std::shared_ptr<sf::SoundBuffer>& buffer) = 0;
};
