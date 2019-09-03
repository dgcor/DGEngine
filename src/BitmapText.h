#pragma once

#include "BitmapFont.h"
#include "DrawableText.h"
#include "SFML/VertexArray2.h"

class BitmapText : public DrawableText
{
private:
	VertexArray2 vertexText;
	std::string text;
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
	BitmapText(const std::shared_ptr<BitmapFont>& font_) : font(font_) {}

	void setFont(const std::shared_ptr<BitmapFont>& font_);

	virtual std::string getText() const { return text; }

	virtual Anchor getAnchor() const noexcept { return anchor; }
	virtual void setAnchor(const Anchor anchor_) noexcept;

	virtual void updateSize(const Game& game) noexcept;

	virtual bool setText(const std::string& str);

	virtual unsigned getLineCount() const noexcept { return lineCount; }

	virtual void setColor(const sf::Color& color_) noexcept;

	virtual const sf::Vector2f& DrawPosition() const noexcept { return drawPos; }
	virtual const sf::Vector2f& Position() const noexcept { return pos; }
	virtual void Position(const sf::Vector2f& position) noexcept;
	virtual sf::Vector2f Size() const noexcept { return size; }
	virtual void Size(const sf::Vector2f& size_) noexcept {}

	virtual sf::FloatRect getLocalBounds() const { return sf::FloatRect(drawPos, size); }
	virtual sf::FloatRect getGlobalBounds() const { return sf::FloatRect(drawPos, size); }

	virtual void setHorizontalAlign(const HorizontalAlign align) noexcept;
	virtual void setVerticalAlign(const VerticalAlign align) noexcept;

	virtual void setHorizontalSpaceOffset(int offset);
	virtual void setVerticalSpaceOffset(int offset);

	virtual bool Visible() const noexcept { return visible; }
	virtual void Visible(bool visible_) noexcept { visible = visible_; }

	virtual void draw(const Game& game, sf::RenderTarget& target) const;

	virtual bool getProperty(const std::string_view prop, Variable& var) const;
};
