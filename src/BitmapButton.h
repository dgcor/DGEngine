#pragma once

#include "Actions/Action.h"
#include "Button.h"
#include <memory>
#include <SFML/Audio/SoundBuffer.hpp>
#include <string>

class BitmapButton : public Button
{
private:
	sf::Sprite sprite;
	bool enabled{ true };
	Anchor anchor{ Anchor::Top | Anchor::Left };
	std::shared_ptr<Action> clickAction;
	std::shared_ptr<Action> doubleClickAction;
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
	bool hovered{ false };
	bool clickUp{ false };
	bool beingDragged{ false };
	bool wasClicked{ false };
	bool visible{ true };
	bool resizable{ false };
	bool captureScrollEvent{ false };

public:
	sf::FloatRect getLocalBounds() const { return sprite.getLocalBounds(); }
	sf::FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); }
	void setTexture(const sf::Texture& texture, bool resetRect = false) { sprite.setTexture(texture, resetRect); }
	void setTextureRect(const sf::IntRect& rectangle) { sprite.setTextureRect(rectangle); }

	bool getResizable() const { return resizable; }
	void setResizable(bool resizable_) { resizable = resizable_; }

	bool getCaptureScrollEvent() const { return resizable; }
	void setCaptureScrollEvent(bool captureScroll) { captureScrollEvent = captureScroll; }

	virtual void setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action);

	virtual void click(Game& game, bool playSound);
	virtual void enable(bool enable) { enabled = enable; }
	virtual void focus(Game& game) const;
	virtual void focusEnabled(bool focusOnClick_) { focusEnable = true; focusOnClick = focusOnClick_; }
	virtual bool isEnabled() const { return enabled; }
	virtual const sf::Vector2f& DrawPosition() const { return sprite.getPosition(); }
	virtual const sf::Vector2f& Position() const { return sprite.getPosition(); }
	virtual void Position(const sf::Vector2f& position) { sprite.setPosition(position); }
	virtual void setAnchor(const Anchor anchor_) { anchor = anchor_; }
	virtual void updateSize(const Game& game);
	virtual void setClickSound(const std::shared_ptr<sf::SoundBuffer>& buffer) { clickSound = buffer; }
	virtual void setClickUp(bool clickUp_) { clickUp = clickUp_; }
	virtual void setColor(const sf::Color& color_) { sprite.setColor(color_); }
	virtual void setFocusSound(const std::shared_ptr<sf::SoundBuffer>& buffer) { focusSound = buffer; }
	virtual void setOrigin(float x, float y) { sprite.setOrigin(x, y); }
	virtual sf::Vector2f Size() const { return sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height); }
	virtual void Size(const sf::Vector2f& size) { sprite.setTextureRect(sf::IntRect(0, 0, (int)size.x, (int)size.y)); }

	virtual bool Visible() const { return visible; }
	virtual void Visible(bool visible_) { visible = visible_; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual void update(Game& game);

	virtual bool getProperty(const std::string& prop, Variable& var) const;
	virtual const Queryable* getQueryable(const std::string& prop) const { return nullptr; }
};
