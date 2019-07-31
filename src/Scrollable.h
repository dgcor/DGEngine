#pragma once

#include <memory>
#include "SFML/View2.h"
#include "UIObject.h"
#include "Utils/ElapsedTime.h"

class Scrollable : public UIObject
{
private:
	std::weak_ptr<UIObject> uiObj;
	View2 view;
	std::shared_ptr<Action> completeAction;

	ElapsedTime elapsedTime;
	float height{ 0.f };
	float offset{ 0.f };
	bool loop{ false };
	bool pause{ false };
	bool visible{ true };

public:
	Scrollable(std::weak_ptr<UIObject> uiObj_, const sf::Time& speed)
		: uiObj(uiObj_), elapsedTime(speed) {}

	void reset();

	void setSpeed(const sf::Time& speed) noexcept { elapsedTime.timeout = speed; }
	void setOffset(float offset_) noexcept { offset = offset_; }
	void setLoop(bool loop_) noexcept { loop = loop_; }
	void setPause(bool pause_) noexcept { pause = pause_; }

	virtual std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept;
	virtual bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept;

	virtual Anchor getAnchor() const noexcept { return view.getAnchor(); }
	virtual void setAnchor(const Anchor anchor) noexcept { view.setAnchor(anchor); }
	virtual void updateSize(const Game& game);

	virtual const sf::Vector2f& DrawPosition() const noexcept { return view.getPosition(); }
	virtual const sf::Vector2f& Position() const noexcept { return view.getPosition(); }
	virtual void Position(const sf::Vector2f& position) noexcept { view.setPosition(position); }
	virtual sf::Vector2f Size() const noexcept { return view.getSize(); }
	virtual void Size(const sf::Vector2f& size) { view.setSize(size); }

	virtual bool Visible() const noexcept { return visible; }
	virtual void Visible(bool visible_) noexcept { visible = visible_; }

	virtual void draw(const Game& game, sf::RenderTarget& target) const;

	virtual void update(Game& game);

	void updateView(const Game& game) { view.update(game); }

	virtual bool getProperty(const std::string_view prop, Variable& var) const;
};
