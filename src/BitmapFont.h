#pragma once

#include "Alignment.h"
#include <memory>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class BitmapFont
{
private:
	std::shared_ptr<sf::Texture> texture;
	sf::IntRect chars[256];
	int newLine;
	int space;
	int rows;
	int columns;
	int padding;

	void calculateCharSizes(const sf::Image& img, bool verticalDirection);
	float calculateLineLength(const char* text, int horizSpaceOffset) const;

public:
	BitmapFont(const std::shared_ptr<sf::Texture>& tex, int rows_, int columns_,
		int padding_, bool verticalDirection);
	BitmapFont(const std::shared_ptr<sf::Texture>& tex, int rows_, int columns_,
		int padding_, bool verticalDirection, const sf::Image& img);
	BitmapFont(const std::shared_ptr<sf::Texture>& tex, int rows_, int columns_,
		int padding_, bool verticalDirection, const std::vector<uint8_t>& charSizes);

	int getNewLine() const { return newLine; }

	sf::Vector2f calculateSize(const std::string& text) const;
	sf::Vector2f calculateSize(const std::string& text,
		int horizSpaceOffset, int vertSpaceOffset) const;

	void draw(const sf::Vector2f& pos, const std::string& text, sf::RenderTarget& target,
		sf::RenderStates states, const sf::Color& color) const;
	void draw(const sf::Vector2f& pos, const std::string& text, sf::RenderTarget& target,
		sf::RenderStates states, const sf::Color& color, int horizSpaceOffset,
		int vertSpaceOffset, float sizeX, HorizontalAlign align) const;
};
