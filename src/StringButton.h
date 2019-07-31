#pragma once

#include "BindableText.h"
#include "Button.h"

class StringButton : public Button, public BindableText
{
public:
	using BindableText::BindableText;

	virtual std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept;
	virtual bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept;

	virtual void setColor(const sf::Color& color_) { Text::setColor(color_); }

	virtual void update(Game& game);

	virtual bool getProperty(const std::string_view prop, Variable& var) const;
};
