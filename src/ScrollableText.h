#pragma once

#include "Actions/Action.h"
#include "DrawableText.h"
#include <memory>
#include <SFML/Graphics.hpp>
#include "SFML/View2.h"
#include "UIObject.h"

class ScrollableText : public UIObject
{
private:
	std::unique_ptr<DrawableText> text;
	View2 view;
	std::shared_ptr<Action> completeAction;

	sf::Time frameTime;
	sf::Time currentTime;
	float height{ 0.f };
	bool loop{ false };
	bool pause{ false };
	bool visible{ true };

public:
	ScrollableText(std::unique_ptr<DrawableText> text_, const sf::Time& frameTime_);

	void reset();

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

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual void update(Game& game);

	virtual void updateViewPort(const Game& game) { view.updateViewport(game); }

	virtual bool getProperty(const std::string_view prop, Variable& var) const;
};
