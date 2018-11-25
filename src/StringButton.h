#pragma once

#include "Button.h"
#include "Text.h"

class StringButton : public Button, public Text
{
public:
	virtual std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept;
	virtual bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept;

	virtual void setColor(const sf::Color& color_) { Text::setColor(color_); }

	virtual void update(Game& game);
};
