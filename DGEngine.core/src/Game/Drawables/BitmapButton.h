#pragma once

#include "Button.h"
#include "Image.h"

class BitmapButton : public Button, public Image
{
public:
	using Image::Image;

	void setColor(const sf::Color& color_) override { sprite.setColor(color_); }

	void update(Game& game) override;

	bool getProperty(const std::string_view prop, Variable& var) const override;
};
