#include "BitmapFont.h"
#include <cmath>
#include "Game.h"
#include "SFML/Sprite2.h"
#include "TexturePacks/BitmapFontTexturePack.h"

BitmapFont::BitmapFont(const std::shared_ptr<BitmapFontTexturePack>& charTextures_, int padding_)
	: charTextures(charTextures_), padding(padding_)
{
	newLine = charTextures_->getCharWidth('\n');
	space = charTextures_->getCharWidth(' ');
	tab = charTextures_->getCharWidth('\t');
}

float BitmapFont::calculateLineLength(const char* text, int horizSpaceOffset) const noexcept
{
	auto ch = text[0];
	if (ch == 0 || ch == '\n')
	{
		return 0.f;
	}

	//Temp offsets
	float curX = 0.f;
	bool wasSpace = false;

	//Go through the text
	while (true)
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
			curX += (float)tab;
		}
		else
		{
			//Move over the width of the character + padding
			curX += (float)charTextures->getCharWidth(ch) + (float)(wasSpace ? 0 : padding);
			wasSpace = false;
		}
		text++;
		ch = text[0];
		if (ch == 0 || ch == '\n')
		{
			return curX;
		}
		else
		{
			curX += (float)horizSpaceOffset;
		}
	}
}

sf::Vector2f BitmapFont::calculateSize(const std::string& text) const
{
	return calculateSize(text, 0, 0);
}

sf::Vector2f BitmapFont::calculateSize(const std::string& text,
	int horizSpaceOffset, int vertSpaceOffset, unsigned* lineCount) const
{
	//Temp offsets
	float maxX = 0, curX = 0, curY = 0;
	unsigned numLines = 0;
	bool wasSpace = false;

	auto textPtr = text.c_str();
	auto ch = textPtr[0];
	if (ch != 0)
	{
		numLines++;
	}
	//Go through the text
	while (ch != 0)
	{
		//If the current character is a newline
		if (ch == '\n')
		{
			//Move down
			curY += newLine + vertSpaceOffset;

			//Move back
			if (curX > maxX)
			{
				maxX = curX;
			}
			curX = 0;
			wasSpace = false;
			numLines++;
		}
		else
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
				curX += (float)tab;
			}
			else
			{
				//Move over the width of the character + padding
				curX += (float)charTextures->getCharWidth(ch) + (float)(wasSpace ? 0 : padding);
				wasSpace = false;
			}
			if (textPtr[1] != 0)
			{
				curX += (float)horizSpaceOffset;
			}
		}
		textPtr++;
		ch = textPtr[0];
	}
	if (lineCount != nullptr)
	{
		(*lineCount) = numLines;
	}
	return sf::Vector2f(std::max(maxX, curX), (newLine + curY));
}

void BitmapFont::draw(const sf::Vector2f& pos, const std::string& text,
	const Game& game, sf::RenderTarget& target, const sf::Color& color) const
{
	draw(pos, text, game, target, color, 0, 0, 0.f, HorizontalAlign::Left);
}

void BitmapFont::draw(const sf::Vector2f& pos, const std::string& text,
	const Game& game, sf::RenderTarget& target, const sf::Color& color,
	int horizSpaceOffset, int vertSpaceOffset, float sizeX, HorizontalAlign align) const
{
	SpriteShaderCache spriteCache;
	Sprite2 sprite(charTextures->getTexture(), palette);
	if (color == sf::Color::White)
	{
		sprite.setColor(defaultColor);
	}
	else
	{
		sprite.setColor(color);
	}

	//Temp offsets
	float curX = pos.x, curY = pos.y;
	bool wasSpace = false;

	if (align == HorizontalAlign::Center)
	{
		curX += std::round((sizeX / 2.f) - (calculateLineLength(text.data(), horizSpaceOffset) / 2.f));
	}
	else if (align == HorizontalAlign::Right)
	{
		curX += (sizeX - calculateLineLength(text.data(), horizSpaceOffset));
	}

	//Go through the text
	for (size_t i = 0; i < text.size(); i++)
	{
		auto ch = text[i];
		//If the current character is a newline
		if (ch == '\n')
		{
			//Move down
			curY += newLine + vertSpaceOffset;

			//Move back
			curX = pos.x;
			if (align == HorizontalAlign::Center)
			{
				curX += std::round((sizeX / 2.f) - (calculateLineLength(text.data() + i + 1, horizSpaceOffset) / 2.f));
			}
			else if (align == HorizontalAlign::Right)
			{
				curX += (sizeX - calculateLineLength(text.data() + i + 1, horizSpaceOffset));
			}
			wasSpace = false;
		}
		else
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
				curX += (float)tab;
			}
			else
			{
				TextureInfo ti;
				charTextures->get((size_t)ch, ti);

				//Show the character
				sprite.setTexture(*ti.texture);
				sprite.setTextureRect(ti.textureRect);
				sprite.setPosition(curX, curY);
				sprite.draw(target, game.Shaders().Sprite, spriteCache);

				//Move over the width of the character + padding
				curX += (float)ti.textureRect.width + (float)(wasSpace ? 0 : padding);
				wasSpace = false;
			}
			curX += (float)horizSpaceOffset;
		}
	}
}
