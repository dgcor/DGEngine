#pragma once

#include "DrawableText.h"

class Text : public virtual UIObject
{
protected:
	std::unique_ptr<DrawableText> text;
	std::shared_ptr<Action> changeAction;
	bool triggerOnChange{ false };

public:
	Text(std::unique_ptr<DrawableText> text_) : text(std::move(text_)) {}
	~Text() = default;

	DrawableText* getDrawableText() noexcept { return text.get(); }

	std::string getText() const { return text->getText(); }
	void setText(const std::string& text_) { triggerOnChange = text->setText(text_); }

	sf::FloatRect getLocalBounds() const { return text->getLocalBounds(); }
	sf::FloatRect getGlobalBounds() const { return text->getGlobalBounds(); }

	void setColor(const sf::Color& color) { text->setColor(color); }
	void setHorizontalAlign(const HorizontalAlign align) { text->setHorizontalAlign(align); }
	void setVerticalAlign(const VerticalAlign align) { text->setVerticalAlign(align); }
	void setHorizontalSpaceOffset(int offset) { text->setHorizontalSpaceOffset(offset); }
	void setVerticalSpaceOffset(int offset) { text->setVerticalSpaceOffset(offset); }

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
