#pragma once

#include "InputEvent.h"
#include <memory>
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/System/Clock.hpp"
#include "UIObject.h"

class Button : public virtual UIObject
{
protected:
	std::shared_ptr<Action> clickAction;
	std::shared_ptr<Action> rightClickAction;
	std::shared_ptr<Action> doubleClickAction;
	std::shared_ptr<Action> clickDragAction;
	std::shared_ptr<Action> clickInAction;
	std::shared_ptr<Action> clickOutAction;
	std::shared_ptr<Action> focusAction;
	std::shared_ptr<Action> hoverEnterAction;
	std::shared_ptr<Action> hoverLeaveAction;
	const sf::SoundBuffer* clickSound{ nullptr };
	const sf::SoundBuffer* focusSound{ nullptr };
	sf::Clock mouseDblClickClock;
	bool enabled{ true };
	bool focusEnable{ false };
	bool focusOnClick{ false };
	bool hovered{ false };
	bool clickUp{ false };
	bool beingDragged{ false };
	bool wasLeftClicked{ false };
	bool wasRightClicked{ false };
	InputEventType captureInputEvents{ InputEventType::None };

	bool click(Game& game, bool playSound, const std::shared_ptr<Action>& action);

	void onHover(Game& game, bool contains);
	void onMouseButtonPressed(Game& game, bool contains);
	void onMouseButtonReleased(Game& game, bool contains);
	void onMouseMoved(Game& game);
	void onTouchBegan(Game& game, bool contains);
	void onTouchEnded(Game& game, bool contains);

	void updateEvents(Game& game, bool contains);

public:
	InputEventType getCaptureInputEvents() const noexcept { return captureInputEvents; }
	void setCaptureInputEvents(InputEventType e) noexcept { captureInputEvents = e; }

	std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept override;
	bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept override;

	bool click(Game& game, bool playSound);
	bool rightClick(Game& game, bool playSound);
	void enable(bool enable) noexcept { enabled = enable; }
	void focus(Game& game) const;
	void focusEnabled(bool focusOnClick_) noexcept { focusEnable = true; focusOnClick = focusOnClick_; }
	bool isEnabled() const noexcept { return enabled; }
	void setClickSound(const sf::SoundBuffer* buffer) noexcept { clickSound = buffer; }
	void setClickUp(bool clickUp_) noexcept { clickUp = clickUp_; }
	void setFocusSound(const sf::SoundBuffer* buffer) noexcept { focusSound = buffer; }

	virtual void setColor(const sf::Color& color_) = 0;

	bool getProperty(const std::string_view prop, Variable& var) const override;
};
