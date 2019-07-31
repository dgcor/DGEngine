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

	virtual std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept;
	virtual bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept;

	virtual Anchor getAnchor() const { return text->getAnchor(); }
	virtual void setAnchor(const Anchor anchor) { text->setAnchor(anchor); }
	virtual void updateSize(const Game& game) { text->updateSize(game); }

	virtual const sf::Vector2f& DrawPosition() const { return text->DrawPosition(); }
	virtual const sf::Vector2f& Position() const { return text->Position(); }
	virtual void Position(const sf::Vector2f& pos) { text->Position(pos); }
	virtual sf::Vector2f Size() const { return text->Size(); }
	virtual void Size(const sf::Vector2f& size) { text->Size(size); }

	virtual bool Visible() const { return text->Visible(); }
	virtual void Visible(bool visible) { text->Visible(visible); }

	virtual void draw(const Game& game, sf::RenderTarget& target) const
	{
		text->draw(game, target);
	}

	virtual void update(Game& game);

	virtual bool getProperty(const std::string_view prop, Variable& var) const
	{
		return text->getProperty(prop, var);
	}
};
