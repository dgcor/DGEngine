#pragma once

#include "Button.h"
#include "Text2.h"

class StringButton : public Button, public Text2
{
public:
	virtual std::shared_ptr<Action> getAction(uint16_t nameHash16);
	virtual bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action);

	virtual void setColor(const sf::Color& color_) { Text2::setColor(color_); }

	virtual void update(Game& game);
};
