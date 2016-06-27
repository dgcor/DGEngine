#include "BitmapFont.h"
#include <cmath>
#include "Game.h"

// This piece of code was originally from Lazy Foo' Productions (http://lazyfoo.net/)
void BitmapFont::calculateCharSizes(const sf::Image& img, bool verticalDirection)
{
	//Set the background color
	auto bgColor = img.getPixel(0, 0);

	//Set the cell dimensions
	int cellW = img.getSize().x / columns;
	int cellH = img.getSize().y / rows;

	//New line variables
	int top = cellH;
	int baseA = cellH;

	//The current character we're setting
	int currentChar = 0;

	//Go through the cell rows
	int iRow = 0;
	int iCol = 0;
	while (currentChar < 256)
	{
		//Set the character offset
		chars[currentChar].left = cellW * iCol;
		chars[currentChar].top = cellH * iRow;

		//Set the dimensions of the character
		chars[currentChar].width = cellW;
		chars[currentChar].height = cellH;

		//Find Left Side
		//Go through pixel columns
		for (int pCol = 0; pCol < cellW; pCol++)
		{
			//Go through pixel rows
			for (int pRow = 0; pRow < cellH; pRow++)
			{
				//Get the pixel offsets
				int pX = (cellW * iCol) + pCol;
				int pY = (cellH * iRow) + pRow;

				//If a non colorkey pixel is found
				if (img.getPixel(pX, pY) != bgColor)
				{
					//Set the x offset
					chars[currentChar].left = pX;

					//Break the loops
					pCol = cellW;
					pRow = cellH;
				}
			}
		}

		//Find Right Side
		//Go through pixel columns
		for (int pColW = cellW - 1; pColW >= 0; pColW--)
		{
			//Go through pixel rows
			for (int pRowW = 0; pRowW < cellH; pRowW++)
			{
				//Get the pixel offsets
				int pX = (cellW * iCol) + pColW;
				int pY = (cellH * iRow) + pRowW;

				//If a non colorkey pixel is found
				if (img.getPixel(pX, pY) != bgColor)
				{
					//Set the width
					chars[currentChar].width = (pX - chars[currentChar].left) + 1;

					//Break the loops
					pColW = -1;
					pRowW = cellH;
				}
			}
		}

		//Find Top
		//Go through pixel rows
		for (int pRow = 0; pRow < cellH; pRow++)
		{
			//Go through pixel columns
			for (int pCol = 0; pCol < cellW; pCol++)
			{
				//Get the pixel offsets
				int pX = (cellW * iCol) + pCol;
				int pY = (cellH * iRow) + pRow;

				//If a non colorkey pixel is found
				if (img.getPixel(pX, pY) != bgColor)
				{
					//If new top is found
					if (pRow < top)
					{
						top = pRow;
					}

					//Break the loops
					pCol = cellW;
					pRow = cellH;
				}
			}
		}

		//Find Bottom of A
		if (currentChar == 'A')
		{
			//Go through pixel rows
			for (int pRow = cellH - 1; pRow >= 0; pRow--)
			{
				//Go through pixel columns
				for (int pCol = 0; pCol < cellW; pCol++)
				{
					//Get the pixel offsets
					int pX = (cellW * iCol) + pCol;
					int pY = (cellH * iRow) + pRow;

					//If a non colorkey pixel is found
					if (img.getPixel(pX, pY) != bgColor)
					{
						//Bottom of a is found
						baseA = pRow;

						//Break the loops
						pCol = cellW;
						pRow = -1;
					}
				}
			}
		}

		//Go to the next character
		currentChar++;

		if (verticalDirection == false)
		{
			iCol++;
			if (iCol >= columns)
			{
				iCol = 0;
				iRow++;
			}
		}
		else
		{
			iRow++;
			if (iRow >= rows)
			{
				iRow = 0;
				iCol++;
			}
		}
	}

	//Calculate space
	space = cellW / 2;

	//Calculate new line
	newLine = baseA - top;

	//Lop off excess top pixels
	for (auto& chrRect : chars)
	{
		chrRect.top += top;
		chrRect.height -= top;
	}
}

BitmapFont::BitmapFont(const std::shared_ptr<sf::Texture>& tex, int rows_, int columns_,
	int padding_, bool verticalDirection)
	: texture(tex), rows(rows_), columns(columns_), padding(padding_)
{
	this->calculateCharSizes(texture->copyToImage(), verticalDirection);
}

BitmapFont::BitmapFont(const std::shared_ptr<sf::Texture>& tex, int rows_, int columns_,
	int padding_, bool verticalDirection, const sf::Image& img)
	: texture(tex), rows(rows_), columns(columns_), padding(padding_)
{
	this->calculateCharSizes(img, verticalDirection);
}

BitmapFont::BitmapFont(const std::shared_ptr<sf::Texture>& tex, int rows_, int columns_,
	int padding_, bool verticalDirection, const std::vector<uint8_t>& charSizes)
	: texture(tex), rows(rows_), columns(columns_), padding(padding_)
{
	if (charSizes.size() == 258)
	{
		int cellW = tex->getSize().x / columns;
		int cellH = tex->getSize().y / rows;
		int iRow = 0;
		int iCol = 0;
		for (auto i = 0; i < 256; i++)
		{
			chars[i].left = cellW * iCol;
			chars[i].top = cellH * iRow;
			chars[i].width = charSizes[i + 2];
			chars[i].height = cellH;

			if (verticalDirection == false)
			{
				iCol++;
				if (iCol >= columns)
				{
					iCol = 0;
					iRow++;
				}
			}
			else
			{
				iRow++;
				if (iRow >= rows)
				{
					iRow = 0;
					iCol++;
				}
			}
		}

		space = charSizes[0];
		newLine = charSizes[1];
	}
	else if (charSizes.size() == 256)
	{
		int cellW = tex->getSize().x / columns;
		int cellH = tex->getSize().y / rows;
		int iRow = 0;
		int iCol = 0;
		for (auto i = 0; i < 256; i++)
		{
			chars[i].left = cellW * iCol;
			chars[i].top = cellH * iRow;
			chars[i].width = charSizes[i];
			chars[i].height = cellH;

			if (verticalDirection == false)
			{
				iCol++;
				if (iCol >= columns)
				{
					iCol = 0;
					iRow++;
				}
			}
			else
			{
				iRow++;
				if (iRow >= rows)
				{
					iRow = 0;
					iCol++;
				}
			}
		}

		space = charSizes[' '];
		newLine = charSizes['\n'];
	}
	else
	{
		this->calculateCharSizes(tex->copyToImage(), verticalDirection);
	}
}

float BitmapFont::calculateLineLength(const char* text) const
{
	//Temp offsets
	float curX = 0;
	bool wasSpace = false;

	//Go through the text
	while (true)
	{
		auto ch = text[0];
		if (ch == 0 || ch == '\n')
		{
			return curX;
		}
		//If the current character is a space
		if (ch == ' ')
		{
			//Move over
			curX += (float)space;
			wasSpace = true;
		}
		//If the current character is a tab
		else if (ch == '\t')
		{
			//Move over
			curX += (float)chars['\t'].width;
		}
		else
		{
			//Get the ASCII value of the character
			int ascii = (unsigned char)ch;

			//Move over the width of the character + padding
			curX += (float)chars[ascii].width + (float)(wasSpace ? 0 : padding);
			wasSpace = false;
		}
		text++;
	}
}

sf::Vector2f BitmapFont::calculateSize(const std::string& text) const
{
	return calculateSize(text, (float)newLine);
}

sf::Vector2f BitmapFont::calculateSize(const std::string& text, const float newLine_) const
{
	//Temp offsets
	float maxX = 0, curX = 0, curY = 0;
	bool wasSpace = false;

	//Go through the text
	for (auto ch : text)
	{
		//If the current character is a space
		if (ch == ' ')
		{
			//Move over
			curX += (float)space;
			wasSpace = true;
		}
		//If the current character is a tab
		else if (ch == '\t')
		{
			//Move over
			curX += (float)chars['\t'].width;
		}
		//If the current character is a newline
		else if (ch == '\n')
		{
			//Move down
			curY += newLine_;

			//Move back
			if (curX > maxX)
			{
				maxX = curX;
			}
			curX = 0;
			wasSpace = false;
		}
		else
		{
			//Get the ASCII value of the character
			int ascii = (unsigned char)ch;

			//Move over the width of the character + padding
			curX += (float)chars[ascii].width + (float)(wasSpace ? 0 : padding);
			wasSpace = false;
		}
	}

	return sf::Vector2f(std::max(maxX, curX), (newLine_ + curY));
}

void BitmapFont::draw(const sf::Vector2f& pos, const std::string& text, sf::RenderTarget& target,
	sf::RenderStates states, const sf::Color& color) const
{
	draw(pos, text, target, states, color, (float)newLine, 0.f, HorizontalAlign::Left);
}

void BitmapFont::draw(const sf::Vector2f& pos, const std::string& text, sf::RenderTarget& target,
	sf::RenderStates states, const sf::Color& color, const float newLine_,
	const float sizeX, const HorizontalAlign align) const
{
	sf::Sprite sprite(*texture);
	sprite.setColor(color);

	//Temp offsets
	float curX = pos.x, curY = pos.y;
	bool wasSpace = false;

	if (align == HorizontalAlign::Center)
	{
		curX += std::round((sizeX / 2.f) - (calculateLineLength(&text.c_str()[0]) / 2.f));
	}
	else if (align == HorizontalAlign::Right)
	{
		curX += (sizeX - calculateLineLength(&text.c_str()[0]));
	}

	//Go through the text
	for (size_t i = 0; i < text.size(); i++)
	{
		auto ch = text[i];
		//If the current character is a space
		if (ch == ' ')
		{
			//Move over
			curX += (float)space;
			wasSpace = true;
		}
		//If the current character is a tab
		else if (ch == '\t')
		{
			//Move over
			curX += (float)chars['\t'].width;
		}
		//If the current character is a newline
		else if (ch == '\n')
		{
			//Move down
			curY += newLine_;

			//Move back
			curX = pos.x;
			if (align == HorizontalAlign::Center)
			{
				curX += std::round((sizeX / 2.f) - (calculateLineLength(&text.c_str()[i + 1]) / 2.f));
			}
			else if (align == HorizontalAlign::Right)
			{
				curX += (sizeX - calculateLineLength(&text.c_str()[i + 1]));
			}
			wasSpace = false;
		}
		else
		{
			//Get the ASCII value of the character
			int ascii = (unsigned char)ch;

			//Show the character
			sprite.setPosition(curX, curY);
			sprite.setTextureRect(chars[ascii]);
			target.draw(sprite, states);

			//Move over the width of the character + padding
			curX += (float)chars[ascii].width + (float)(wasSpace ? 0 : padding);
			wasSpace = false;
		}
	}
}
