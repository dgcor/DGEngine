#pragma once

#include "Button.h"
#include "Image.h"

class BitmapButton : public Button, public Image
{
public:
	std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept override
	{
		return Button::getAction(nameHash16);
	}
	bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept override
	{
		return Button::setAction(nameHash16, action);
	}

	void setColor(const sf::Color& color_) override { sprite.setColor(color_); }

	void update(Game& game) override;

	bool getProperty(const std::string_view prop, Variable& var) const override;
};
