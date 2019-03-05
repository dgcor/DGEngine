#pragma once

#include "Button.h"
#include "Image.h"

class BitmapButton : public Button, public Image
{
public:
	virtual std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept
	{
		return Button::getAction(nameHash16);
	}
	virtual bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept
	{
		return Button::setAction(nameHash16, action);
	}

	virtual void setColor(const sf::Color& color_) { sprite.setColor(color_); }

	virtual void update(Game& game);

	virtual bool getProperty(const std::string_view prop, Variable& var) const;
};
