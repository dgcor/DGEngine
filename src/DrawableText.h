#pragma once

#include "Alignment.h"
#include <string>
#include "UIObject.h"

class DrawableText : public UIObject
{
public:
	virtual std::string getText() const = 0;
	virtual void setText(const std::string& string) = 0;
	virtual sf::FloatRect getLocalBounds() const = 0;
	virtual sf::FloatRect getGlobalBounds() const = 0;
	virtual void setColor(const sf::Color& color) = 0;
	virtual void setLineSpacing(unsigned lineSpacing) = 0;
	virtual void setHorizontalAlign(const HorizontalAlign align) = 0;
	virtual void setVerticalAlign(const VerticalAlign align) = 0;
};
