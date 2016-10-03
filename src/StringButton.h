#pragma once

#include "Actions/Action.h"
#include "Button.h"
#include <memory>
#include <SFML/Audio/SoundBuffer.hpp>
#include <string>
#include "Text2.h"

class StringButton : public Button
{
private:
	std::unique_ptr<Text2> text;
	bool enabled{ true };
	std::shared_ptr<Action> clickAction;
	std::shared_ptr<Action> doubleClickAction;
	std::shared_ptr<Action> toggleAction;
	std::shared_ptr<Action> clickDragAction;
	std::shared_ptr<Action> clickInAction;
	std::shared_ptr<Action> clickOutAction;
	std::shared_ptr<Action> focusAction;
	std::shared_ptr<Action> hoverEnterAction;
	std::shared_ptr<Action> hoverLeaveAction;
	std::shared_ptr<sf::SoundBuffer> clickSound;
	std::shared_ptr<sf::SoundBuffer> focusSound;
	bool focusEnable{ false };
	bool focusOnClick{ false };
	bool toggled{ false };
	bool hovered{ false };
	bool clickUp{ false };
	bool beingDragged{ false };
	bool wasClicked{ false };
	bool visible{ true };

public:
	std::string getText() const { return text->getText(); }
	void setText(std::unique_ptr<Text2> text_) { text = std::move(text_); }
	void setText(const std::string& text_) { text->setText(text_); }

	DrawableText* getDrawableText() { return text->getDrawableText(); }

	sf::FloatRect getLocalBounds() const { return text->getLocalBounds(); }
	sf::FloatRect getGlobalBounds() const { return text->getGlobalBounds(); }

	virtual void click(Game& game, bool playSound);
	virtual void enable(bool enable) { enabled = enable; }
	virtual void focus(Game& game);
	virtual void focusEnabled(bool focusOnClick_) { focusEnable = true; focusOnClick = focusOnClick_; }
	virtual bool isEnabled() { return enabled; }
	virtual const sf::Vector2f& DrawPosition() const { return text->DrawPosition(); }
	virtual const sf::Vector2f& Position() const { return text->Position(); }
	virtual void Position(const sf::Vector2f& position) { text->Position(position); }
	virtual void setAnchor(const Anchor anchor) { text->setAnchor(anchor); }
	virtual void updateSize(const Game& game) { text->updateSize(game); }
	virtual void setClickAction(const std::shared_ptr<Action>& action) { clickAction = action; }
	virtual void setClickDragAction(const std::shared_ptr<Action>& action) { clickDragAction = action; }
	virtual void setClickInAction(const std::shared_ptr<Action>& action) { clickInAction = action; }
	virtual void setClickOutAction(const std::shared_ptr<Action>& action) { clickOutAction = action; }
	virtual void setClickSound(const std::shared_ptr<sf::SoundBuffer>& buffer) { clickSound = buffer; }
	virtual void setClickUp(bool clickUp_) { clickUp = clickUp_; }
	virtual void setColor(const sf::Color& color_) { text->setColor(color_); }
	virtual void setDoubleClickAction(const std::shared_ptr<Action>& action) { doubleClickAction = action; }
	virtual void setFocusAction(const std::shared_ptr<Action>& action) { focusAction = action; }
	virtual void setFocusSound(const std::shared_ptr<sf::SoundBuffer>& buffer) { focusSound = buffer; }
	void setHorizontalAlign(const HorizontalAlign align) { text->setHorizontalAlign(align); }
	virtual void setHoverEnterAction(const std::shared_ptr<Action>& action) { hoverEnterAction = action; }
	virtual void setHoverLeaveAction(const std::shared_ptr<Action>& action) { hoverLeaveAction = action; }
	virtual void setToggle(bool toggle) { toggled = toggle; }
	virtual void setToggleAction(const std::shared_ptr<Action>& action) { toggleAction = action; }
	void setVerticalAlign(const VerticalAlign align) { text->setVerticalAlign(align); }
	virtual sf::Vector2f Size() const { return text->Size(); }
	virtual void Size(const sf::Vector2f& size) {}

	virtual bool Visible() const { return visible; }
	virtual void Visible(bool visible_) { visible = visible_; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual void update(Game& game);

	virtual bool getProperty(const std::string& prop, Variable& var) const
	{
		return text->getProperty(prop, var);
	}
};
