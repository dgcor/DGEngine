#pragma once

#include "DrawableText.h"
#include "Game/ActionQueryable.h"
#include "Resources/Font.h"

class Text : public virtual ActionQueryable, public virtual UIObject
{
protected:
	std::unique_ptr<DrawableText> text;
	std::shared_ptr<Action> changeAction;
	bool triggerOnChange{ false };

public:
	Text(const Font& font, unsigned int characterSize = 0);
	~Text() = default;

	// allow copy, but not move
	Text(Text const& other) : text(other.text->clone()) {}
	Text(Text&& other) = delete;
	Text& operator=(Text const& other)
	{
		text = other.text->clone();
		return *this;
	}
	Text& operator=(Text&& other) = delete;

	auto getDrawableText() noexcept { return text.get(); }

	auto getText() const { return text->getText(); }
	void setText(const std::string& text_) { triggerOnChange = text->setText(text_); }

	auto getLocalBounds() const { return text->getLocalBounds(); }
	auto getGlobalBounds() const { return text->getGlobalBounds(); }

	auto getLineCount() const { return text->getLineCount(); }
	void setColor(const sf::Color& color) { text->setColor(color); }

	void setFont(const Font& font);

	void setHorizontalAlign(const HorizontalAlign align) { text->setHorizontalAlign(align); }
	void setVerticalAlign(const VerticalAlign align) { text->setVerticalAlign(align); }

	void setHorizontalSpaceOffset(int offset) { text->setHorizontalSpaceOffset(offset); }
	void setVerticalSpaceOffset(int offset) { text->setVerticalSpaceOffset(offset); }

	HorizontalAlign getHorizontalAlign() const { return text->getHorizontalAlign(); }
	VerticalAlign getVerticalAlign() const { return text->getVerticalAlign(); }

	std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept override;
	bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept override;

	Anchor getAnchor() const override { return text->getAnchor(); }
	void setAnchor(const Anchor anchor) override { text->setAnchor(anchor); }
	void updateSize(const Game& game) override { text->updateSize(game); }

	const sf::Vector2f& DrawPosition() const override { return text->DrawPosition(); }
	const sf::Vector2f& Position() const override { return text->Position(); }
	void Position(const sf::Vector2f& pos) override { text->Position(pos); }
	sf::Vector2f Size() const override { return text->Size(); }
	void Size(const sf::Vector2f& size) override { text->Size(size); }

	bool Visible() const override { return text->Visible(); }
	void Visible(bool visible) override { text->Visible(visible); }

	void draw(const Game& game, sf::RenderTarget& target) const override
	{
		text->draw(game, target);
	}

	void update(Game& game) override;

	bool getProperty(const std::string_view prop, Variable& var) const override
	{
		return text->getProperty(prop, var);
	}
};
