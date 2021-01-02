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
	unsigned int charSize{ 0 };
	bool visible{ true };

	void calculateDrawPosition();
	void updateCharSize();
	void updateColor();

public:
	StringText(const std::shared_ptr<FreeTypeFont>& font_, unsigned int characterSize = 30);

	Anchor getAnchor() const noexcept override { return anchor; }
	void setAnchor(const Anchor anchor_) override;
	void updateSize(const Game& game) override;

	bool setText(const std::string& utf8Str) override;

	unsigned getLineCount() const noexcept override { return text.getLineCount(); }

	void setFont(const std::shared_ptr<FreeTypeFont>& font_);
	void setCharacterSize(unsigned int size);
	void setStyle(sf::Uint32 style) { text.setStyle(style); }
	void setColor(const sf::Color& color_) override;
	void setOutlineColor(const sf::Color& color_) { text.setOutlineColor(color_); }
	void setOutlineThickness(float thickness) { text.setOutlineThickness(thickness); }

	std::string getText() const override;
	const sf::Font* getFont() const { return text.getFont(); }
	unsigned int getCharacterSize() const { return text.getCharacterSize(); }
	sf::Uint32 getStyle() const { return text.getStyle(); }
	const sf::Color& getColor() const { return text.getFillColor(); }
	const sf::Color& getOutlineColor() const { return text.getOutlineColor(); }
	float getOutlineThickness() const { return text.getOutlineThickness(); }
	sf::Vector2f findCharacterPos(std::size_t index) const { return text.findCharacterPos(index); }
	sf::FloatRect getLocalBounds() const override { return text.getLocalBounds(); }
	sf::FloatRect getGlobalBounds() const override { return text.getGlobalBounds(); }

	const sf::Vector2f& DrawPosition() const override { return text.getPosition(); }
	const sf::Vector2f& Position() const noexcept override { return pos; }
	void Position(const sf::Vector2f& position) override;
	sf::Vector2f Size() const override;
	void Size(const sf::Vector2f& size) noexcept override {}

	void setHorizontalAlign(const HorizontalAlign align) override;
	void setVerticalAlign(const VerticalAlign align) override;

	void setHorizontalSpaceOffset(int offset) noexcept override;
	void setVerticalSpaceOffset(int offset) noexcept override;

	bool Visible() const noexcept override { return visible; }
	void Visible(bool visible_) noexcept override { visible = visible_; }

	void draw(const Game& game, sf::RenderTarget& target) const override;

	bool getProperty(const std::string_view prop, Variable& var) const override;
};
