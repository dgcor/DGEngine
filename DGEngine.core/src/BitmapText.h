#pragma once

#include "BitmapFont.h"
#include "DrawableText.h"
#include "SFML/VertexArray2.h"

class BitmapText : public DrawableText
{
private:
	VertexArray2 vertexText;
	std::string text;	// utf8
	std::shared_ptr<BitmapFont> font;
	sf::Vector2f pos;
	sf::Vector2f drawPos;
	sf::Vector2f size;
	HorizontalAlign horizAlign{ HorizontalAlign::Left };
	VerticalAlign vertAlign{ VerticalAlign::Bottom };
	Anchor anchor{ Anchor::Top | Anchor::Left };
	sf::Color color{ sf::Color::White };
	int horizSpaceOffset{ 0 };
	int vertSpaceOffset{ 0 };
	unsigned lineCount{ 0 };
	bool visible{ true };

	void calculateDrawPosition() noexcept;
	void calculateSize();
	void updateVertexText();

public:
	BitmapText(const std::shared_ptr<BitmapFont>& font_);

	void setFont(const std::shared_ptr<BitmapFont>& font_);

	std::string getText() const override { return text; }

	Anchor getAnchor() const noexcept override { return anchor; }
	void setAnchor(const Anchor anchor_) noexcept override;

	void updateSize(const Game& game) noexcept override;

	bool setText(const std::string& utf8Str) override;

	unsigned getLineCount() const noexcept override { return lineCount; }

	void setColor(const sf::Color& color_) noexcept override;

	const sf::Vector2f& DrawPosition() const noexcept override { return drawPos; }
	const sf::Vector2f& Position() const noexcept override { return pos; }
	void Position(const sf::Vector2f& position) noexcept override;
	sf::Vector2f Size() const noexcept override { return size; }
	void Size(const sf::Vector2f& size_) noexcept override {}

	sf::FloatRect getLocalBounds() const override { return sf::FloatRect(drawPos, size); }
	sf::FloatRect getGlobalBounds() const override { return sf::FloatRect(drawPos, size); }

	void setHorizontalAlign(const HorizontalAlign align) noexcept override;
	void setVerticalAlign(const VerticalAlign align) noexcept override;

	void setHorizontalSpaceOffset(int offset) override;
	void setVerticalSpaceOffset(int offset) override;

	bool Visible() const noexcept override { return visible; }
	void Visible(bool visible_) noexcept override { visible = visible_; }

	void draw(const Game& game, sf::RenderTarget& target) const override;

	bool getProperty(const std::string_view prop, Variable& var) const override;
};
