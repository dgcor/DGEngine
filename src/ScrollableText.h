#pragma once

#include "Actions/Action.h"
#include "DrawableText.h"
#include <memory>
#include <SFML/Graphics.hpp>
#include "UIObject.h"
#include "View2.h"

class ScrollableText : public UIObject
{
private:
	std::unique_ptr<DrawableText> text;
	View2 view;
	Anchor anchor{ Anchor::Top | Anchor::Left };
	std::shared_ptr<Action> action;

	sf::Time frameTime;
	sf::Time currentTime;
	float height{ 0.f };
	bool loop{ false };
	bool pause{ false };
	bool visible{ true };

public:
	ScrollableText(std::unique_ptr<DrawableText> text_, const sf::Time& frameTime_);

	void reset()
	{
		text->Position(sf::Vector2f(0.f, 0.f));
		const auto& vSize = view.getSize();
		view.reset(sf::FloatRect(0, -vSize.y, vSize.x, vSize.y));
		height = vSize.y;
	}

	void updateViewPort(const Game& game);

	void setAction(const std::shared_ptr<Action>& action_) { action = action_; }
	void setSpeed(const std::shared_ptr<Action>& action_) { action = action_; }

	void setLoop(bool loop_) { loop = loop_; }
	void setPause(bool pause_) { pause = pause_; }

	virtual void setAnchor(const Anchor anchor_) { anchor = anchor_; }
	virtual void updateSize(const Game& game);

	virtual const sf::Vector2f& DrawPosition() const { return view.getPosition(); }
	virtual const sf::Vector2f& Position() const { return view.getPosition(); }
	virtual void Position(const sf::Vector2f& position) { view.setPosition(position); }
	virtual sf::Vector2f Size() const { return view.getSize(); }
	virtual void Size(const sf::Vector2f& size) { view.setSize(size); }

	virtual bool Visible() const { return visible; }
	virtual void Visible(bool visible_) { visible = visible_; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual void update(Game& game);

	virtual bool getProperty(const std::string& prop, Variable& var) const;
};
