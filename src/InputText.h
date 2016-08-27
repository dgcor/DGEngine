#pragma once

#include "Actions/Action.h"
#include "DrawableText.h"
#include <memory>
#include <regex>
#include <SFML/Graphics.hpp>
#include "UIObject.h"

class InputText : public UIObject
{
private:
	std::unique_ptr<DrawableText> text;
	std::shared_ptr<Action> actionChange;
	std::shared_ptr<Action> actionEnter;
	std::shared_ptr<Action> actionMinSize;
	unsigned minSize;
	unsigned maxSize;
	bool visible{ true };
	bool hasRegex{ false };
	std::regex regex;

public:
	InputText(std::unique_ptr<DrawableText> text_, unsigned minSize_, unsigned maxSize_)
		: text(std::move(text_)), minSize(minSize_), maxSize(maxSize_) {}

	void setText(const std::string& string) { text->setText(string); }
	void setColor(const sf::Color& color) { text->setColor(color); }
	std::string getText() const { return text->getText(); }
	sf::FloatRect getLocalBounds() const { return text->getLocalBounds(); }
	sf::FloatRect getGlobalBounds() const { return text->getGlobalBounds(); }

	virtual void setAnchor(const Anchor anchor_) { text->setAnchor(anchor_); }
	virtual void updateSize(const Game& game) { text->updateSize(game); }

	virtual const sf::Vector2f& DrawPosition() const { return text->DrawPosition(); }
	virtual const sf::Vector2f& Position() const { return text->Position(); }
	virtual void Position(const sf::Vector2f& position) { text->Position(position); }
	virtual sf::Vector2f Size() const { return text->Size(); }
	virtual void Size(const sf::Vector2f& size) {}

	void setActionChange(const std::shared_ptr<Action>& action_) { actionChange = action_; }
	void setActionEnter(const std::shared_ptr<Action>& action_) { actionEnter = action_; }
	void setActionMinSize(const std::shared_ptr<Action>& action_) { actionMinSize = action_; }
	void setRegex(const std::string& regex_)
	{
		hasRegex = false;
		if (regex_.size() > 0)
		{
			regex.assign(regex_, std::regex::ECMAScript);
			hasRegex = true;
		}
	}

	void click(Game& game);

	virtual bool Visible() const { return visible; }
	virtual void Visible(bool visible_) { visible = visible_; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (visible == true)
		{
			target.draw(*text, states);
		}
	}

	virtual void update(Game& game);

	virtual bool getProperty(const std::string& prop, Variable& var) const;
};
