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

	std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept override;
	bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept override;

	Anchor getAnchor() const noexcept override { return view.getAnchor(); }
	void setAnchor(const Anchor anchor) noexcept override { view.setAnchor(anchor); }
	void updateSize(const Game& game) override;

	const sf::Vector2f& DrawPosition() const noexcept override { return view.getPosition(); }
	const sf::Vector2f& Position() const noexcept override { return view.getPosition(); }
	void Position(const sf::Vector2f& position) noexcept override { view.setPosition(position); }
	sf::Vector2f Size() const noexcept override { return view.getSize(); }
	void Size(const sf::Vector2f& size) override { view.setSize(size); }

	bool Visible() const noexcept override { return visible; }
	void Visible(bool visible_) noexcept override { visible = visible_; }

	void draw(const Game& game, sf::RenderTarget& target) const override;

	void update(Game& game) override;

	void updateView(const Game& game) { view.update(game); }

	bool getProperty(const std::string_view prop, Variable& var) const override;
};
