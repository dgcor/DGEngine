#pragma once

#include "Actions/Action.h"
#include "DrawableText.h"
#include <memory>
#include <optional>
#include <regex>
#include <SFML/Graphics.hpp>
#include "UIObject.h"

class InputText : public UIObject
{
private:
	std::unique_ptr<DrawableText> text;
	std::shared_ptr<Action> actionChange;
	std::shared_ptr<Action> actionEnter;
	std::shared_ptr<Action> actionMinLength;
	size_t minLength{ 0 };
	size_t maxLength{ 0 };
	std::optional<Variable> minValue;
	std::optional<Variable> maxValue;
	std::optional<std::regex> regex;

	bool isValidMin(const std::string& str) const noexcept;
	bool isValidMax(const std::string& str) const noexcept;

public:
	InputText(std::unique_ptr<DrawableText> text_) : text(std::move(text_)) {}

	void setText(const std::string& string) { text->setText(string); }
	void setColor(const sf::Color& color) { text->setColor(color); }
	std::string getText() const { return text->getText(); }
	sf::FloatRect getLocalBounds() const { return text->getLocalBounds(); }
	sf::FloatRect getGlobalBounds() const { return text->getGlobalBounds(); }

	virtual std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept;
	virtual bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept;

	virtual void setAnchor(const Anchor anchor_) { text->setAnchor(anchor_); }
	virtual void updateSize(const Game& game) { text->updateSize(game); }

	virtual const sf::Vector2f& DrawPosition() const { return text->DrawPosition(); }
	virtual const sf::Vector2f& Position() const { return text->Position(); }
	virtual void Position(const sf::Vector2f& position) { text->Position(position); }
	virtual sf::Vector2f Size() const { return text->Size(); }
	virtual void Size(const sf::Vector2f& size) noexcept {}

	void setMinLength(size_t length) noexcept { minLength = length; }
	void setMaxLength(size_t length) noexcept { maxLength = length; }
	void setMinValue(const Variable& var) { minValue = var; }
	void setMaxValue(const Variable& var) { maxValue = var; }
	void setRegex(const std::string& regex_);

	void click(Game& game);

	virtual bool Visible() const { return text->Visible(); }
	virtual void Visible(bool visible_) { text->Visible(visible_); }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(*text, states);
	}

	virtual void update(Game& game);

	virtual bool getProperty(const std::string_view prop, Variable& var) const;
};
