#pragma once

#include <SFML/Graphics.hpp>
#include "DrawableText.h"

class StringText : public DrawableText
{
private:
	sf::Text text;
	sf::Vector2f pos;
	Anchor anchor{ Anchor::Top | Anchor::Left };
	HorizontalAlign horizAlign{ HorizontalAlign::Left };
	VerticalAlign vertAlign{ VerticalAlign::Bottom };
	bool visible{ true };

	void calculateDrawPosition();

public:
	StringText(const sf::String& string, const sf::Font& font, unsigned int characterSize = 30) :
		text(string, font, characterSize) {}

	virtual ~StringText() {}

	virtual void setAnchor(const Anchor anchor_)
	{
		if (anchor != anchor_)
		{
			anchor = anchor_;
			calculateDrawPosition();
		}
	}
	virtual void updateSize(const Game& game);

	virtual void setText(const std::string& string)
	{
		text.setString(string);
		calculateDrawPosition();
	}

	void setFont(const sf::Font& font) { text.setFont(font); }
	void setCharacterSize(unsigned int size) { text.setCharacterSize(size); }
	void setStyle(sf::Uint32 style) { text.setStyle(style); }
	virtual void setColor(const sf::Color& color) { text.setColor(color); }

	virtual std::string getText() const { return text.getString().toAnsiString(); }
	const sf::Font* getFont() const { return text.getFont(); }
	unsigned int getCharacterSize() const { return text.getCharacterSize(); }
	sf::Uint32 getStyle() const { return text.getStyle(); }
	const sf::Color& getColor() const { return text.getColor(); }
	sf::Vector2f findCharacterPos(std::size_t index) const { return text.findCharacterPos(index); }
	virtual sf::FloatRect getLocalBounds() const { return text.getLocalBounds(); }
	virtual sf::FloatRect getGlobalBounds() const { return text.getGlobalBounds(); }

	virtual void setHorizontalAlign(const HorizontalAlign align)
	{
		if (horizAlign != align)
		{
			horizAlign = align;
			calculateDrawPosition();
		}
	}

	virtual void setVerticalAlign(const VerticalAlign align)
	{
		if (vertAlign != align)
		{
			vertAlign = align;
			calculateDrawPosition();
		}
	}

	virtual const sf::Vector2f& DrawPosition() const { return text.getPosition(); }
	virtual const sf::Vector2f& Position() const { return pos; }
	virtual void Position(const sf::Vector2f& position)
	{
		pos = position;
		calculateDrawPosition();
	}
	virtual sf::Vector2f Size() const { return sf::Vector2f(text.getLocalBounds().width, text.getLocalBounds().height); }
	virtual void Size(const sf::Vector2f& size) {}
	virtual void setLineSpacing(unsigned lineSpacing) {}

	void setRotation(float angle) { text.setRotation(angle); }
	void setScale(float factorX, float factorY) { text.setScale(factorX, factorY); }
	void setScale(const sf::Vector2f& factors) { text.setScale(factors); }
	void setOrigin(float x, float y) { text.setOrigin(x, y); }
	void setOrigin(const sf::Vector2f& origin) { text.setOrigin(origin); }
	float getRotation() const { return text.getRotation(); }
	const sf::Vector2f& getScale() const { return text.getScale(); }
	const sf::Vector2f& getOrigin() const { return text.getOrigin(); }
	void move(float offsetX, float offsetY) { text.move(offsetX, offsetY); }
	void move(const sf::Vector2f& offset) { text.move(offset); }
	void rotate(float angle) { text.rotate(angle); }
	void scale(float factorX, float factorY) { text.scale(factorX, factorY); }
	void scale(const sf::Vector2f& factor) { text.scale(factor); }
	const sf::Transform& getTransform() const { return text.getTransform(); }
	const sf::Transform& getInverseTransform() const { return text.getInverseTransform(); }

	virtual bool Visible() const { return visible; }
	virtual void Visible(bool visible_) { visible = visible_; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (visible == true)
		{
			target.draw(text, states);
		}
	}

	virtual void update(Game& game) {}

	virtual Variable getProperty(const std::string& prop) const;
};
