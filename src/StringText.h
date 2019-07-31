#pragma once

#include "DrawableText.h"
#include "FreeTypeFont.h"
#include "SFML/Text2.h"

class StringText : public DrawableText
{
private:
	sf::Text text;
	std::shared_ptr<FreeTypeFont> font;
	sf::Vector2f pos;
	Anchor anchor{ Anchor::Top | Anchor::Left };
	HorizontalAlign horizAlign{ HorizontalAlign::Left };
	VerticalAlign vertAlign{ VerticalAlign::Bottom };
	sf::Color color{ sf::Color::White };
	bool visible{ true };

	void calculateDrawPosition();
	void updateColor();

public:
	StringText(const std::shared_ptr<FreeTypeFont>& font_, unsigned int characterSize = 30);

	virtual Anchor getAnchor() const noexcept { return anchor; }
	virtual void setAnchor(const Anchor anchor_);
	virtual void updateSize(const Game& game);

	virtual bool setText(const std::string& str);

	virtual unsigned getLineCount() const noexcept { return text.getLineCount(); }

	void setFont(const std::shared_ptr<FreeTypeFont>& font_);
	void setCharacterSize(unsigned int size) { text.setCharacterSize(size); }
	void setStyle(sf::Uint32 style) { text.setStyle(style); }
	virtual void setColor(const sf::Color& color_);
	void setOutlineColor(const sf::Color& color_) { text.setOutlineColor(color_); }
	void setOutlineThickness(float thickness) { text.setOutlineThickness(thickness); }

	virtual std::string getText() const { return text.getString().toAnsiString(); }
	const sf::Font* getFont() const { return text.getFont(); }
	unsigned int getCharacterSize() const { return text.getCharacterSize(); }
	sf::Uint32 getStyle() const { return text.getStyle(); }
	const sf::Color& getColor() const { return color; }
	const sf::Color& getOutlineColor() const { return text.getOutlineColor(); }
	float getOutlineThickness() const { return text.getOutlineThickness(); }
	sf::Vector2f findCharacterPos(std::size_t index) const { return text.findCharacterPos(index); }
	virtual sf::FloatRect getLocalBounds() const { return text.getLocalBounds(); }
	virtual sf::FloatRect getGlobalBounds() const { return text.getGlobalBounds(); }

	virtual const sf::Vector2f& DrawPosition() const { return text.getPosition(); }
	virtual const sf::Vector2f& Position() const noexcept { return pos; }
	virtual void Position(const sf::Vector2f& position);
	virtual sf::Vector2f Size() const;
	virtual void Size(const sf::Vector2f& size) noexcept {}

	virtual void setHorizontalAlign(const HorizontalAlign align);
	virtual void setVerticalAlign(const VerticalAlign align);

	virtual void setHorizontalSpaceOffset(int offset) noexcept;
	virtual void setVerticalSpaceOffset(int offset) noexcept;

	virtual bool Visible() const noexcept { return visible; }
	virtual void Visible(bool visible_) noexcept { visible = visible_; }

	virtual void draw(const Game& game, sf::RenderTarget& target) const;

	virtual bool getProperty(const std::string_view prop, Variable& var) const;
};
