#pragma once

#include "Button.h"
#include "Image.h"

class BitmapButton : public Button, public Image
{
private:
	bool resizable{ false };

public:
	sf::FloatRect getLocalBounds() const { return sprite.getLocalBounds(); }
	sf::FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); }

	bool getResizable() const { return resizable; }
	void setResizable(bool resizable_) { resizable = resizable_; }

	virtual std::shared_ptr<Action> getAction(uint16_t nameHash16)
	{
		return Button::getAction(nameHash16);
	}
	virtual bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action)
	{
		return Button::setAction(nameHash16, action);
	}

	virtual void updateSize(const Game& game);
	virtual void setColor(const sf::Color& color_) { sprite.setColor(color_); }
	virtual sf::Vector2f Size() const
	{
		return sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
	}
	virtual void Size(const sf::Vector2f& size)
	{
		sprite.setTextureRect(sf::IntRect(0, 0, (int)size.x, (int)size.y));
	}

	virtual void update(Game& game);
};
