#include "BitmapFont.h"
#include <cmath>
#include <SFML/System/Utf.hpp>
#include "TexturePacks/TexturePack.h"

BitmapFont::BitmapFont(const std::shared_ptr<TexturePack>& texturePack_,
	int16_t newLine_, int16_t space_, int16_t tab_) : texturePack(texturePack_)
{
	newLine = newLine_ < 0 ? texturePack_->getWidth('\n') : newLine_;
	space = space_ < 0 ? texturePack_->getWidth(' ') : space_;
	tab = tab_ < 0 ? texturePack_->getWidth('\t') : tab_;
}

const Palette* BitmapFont::getPalette() const noexcept
{
	if (palette != nullptr)
	{
		return palette.get();
	}
	else if (texturePack->getPalette() != nullptr)
	{
		return texturePack->getPalette().get();
	}
	return nullptr;
}

int BitmapFont::getHorizontalSpaceOffset(int horizSpaceOffset) const noexcept
{
	return horizSpaceOffset != 0 ? horizSpaceOffset : defaultHorizSpaceOffset;
}

int BitmapFont::getVerticalSpaceOffset(int vertSpaceOffset) const noexcept
{
	return vertSpaceOffset != 0 ? vertSpaceOffset : defaultVertSpaceOffset;
}

float BitmapFont::calculateLineLength(std::string_view::const_iterator itBegin,
	std::string_view::const_iterator itEnd, int horizSpaceOffset) const
{
	horizSpaceOffset = getHorizontalSpaceOffset(horizSpaceOffset);

	//Temp offsets
	float curX = 0.f;

	for (auto it = itBegin; it < itEnd;)
	{
		sf::Uint32 ch;
		it = sf::Utf8::decode(it, itEnd, ch);

		//If the current character is a newline
		if (ch == '\n')
		{
			return curX;
		}
		//If the current character is a space
		if (ch == ' ')
		{
			//Move over
			curX += (float)space;
		}
		//If the current character is a tab
		else if (ch == '\t')
		{
			//Move over
			curX += (float)tab;
		}
		else
		{
			//Move over the width of the character
			curX += (float)texturePack->getWidth(ch);
		}
		if (it < itEnd)
		{
			curX += (float)horizSpaceOffset;
		}
	}
	return curX;
}

sf::Vector2f BitmapFont::calculateSize(const std::string_view text) const
{
	return calculateSize(text, 0, 0);
}

sf::Vector2f BitmapFont::calculateSize(const std::string_view text,
	int horizSpaceOffset, int vertSpaceOffset, unsigned* lineCount) const
{
	horizSpaceOffset = getHorizontalSpaceOffset(horizSpaceOffset);
	vertSpaceOffset = getHorizontalSpaceOffset(vertSpaceOffset);

	//Temp offsets
	float maxX = 0, curX = 0, curY = 0;
	unsigned numLines = 0;

	auto itBegin = text.begin();
	auto itEnd = text.end();
	for (auto it = itBegin; it < itEnd;)
	{
		if (it == itBegin)
		{
			numLines++;
		}

		sf::Uint32 ch;
		it = sf::Utf8::decode(it, itEnd, ch);

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
			numLines++;
		}
		else
		{
			//If the current character is a space
			if (ch == ' ')
			{
				//Move over
				curX += (float)space;
			}
			//If the current character is a tab
			else if (ch == '\t')
			{
				//Move over
				curX += (float)tab;
			}
			else
			{
				//Move over the width of the character
				curX += (float)texturePack->getWidth(ch);
			}
			if (it < itEnd)
			{
				curX += (float)horizSpaceOffset;
			}
		}
	}
	if (lineCount != nullptr)
	{
		(*lineCount) = numLines;
	}
	return sf::Vector2f(std::max(maxX, curX), (newLine + curY));
}

void BitmapFont::updateVertexString(std::vector<sf::Vertex>& vertexText,
	const std::string_view text, sf::Color color, int horizSpaceOffset,
	int vertSpaceOffset, float sizeX, HorizontalAlign align) const
{
	horizSpaceOffset = getHorizontalSpaceOffset(horizSpaceOffset);
	vertSpaceOffset = getHorizontalSpaceOffset(vertSpaceOffset);

	if (hasPalette() == true)
	{
		color = sf::Color::White;
	}
	else if (color == sf::Color::White)
	{
		color = defaultColor;
	}

	vertexText.clear();
	vertexText.reserve(text.size() * 6);

	//Temp offsets
	float curX = 0.f;
	float curY = 0.f;

	if (align == HorizontalAlign::Center)
	{
		auto textLength = calculateLineLength(text.begin(), text.end(), horizSpaceOffset);
		curX += std::round((sizeX / 2.f) - (textLength / 2.f));
	}
	else if (align == HorizontalAlign::Right)
	{
		auto textLength = calculateLineLength(text.begin(), text.end(), horizSpaceOffset);
		curX += (sizeX - textLength);
	}

	//Iterate the utf8 encoded text
	for (auto it = text.begin(), itEnd = text.end(); it < itEnd;)
	{
		sf::Uint32 ch;
		it = sf::Utf8::decode(it, itEnd, ch);

		//If the current character is a newline
		if (ch == '\n')
		{
			//Move down
			curY += newLine + vertSpaceOffset;

			//Move back
			curX = 0;
			if (align == HorizontalAlign::Center)
			{
				auto textLengh = calculateLineLength(it, itEnd, horizSpaceOffset);
				curX += std::round((sizeX / 2.f) - (textLengh / 2.f));
			}
			else if (align == HorizontalAlign::Right)
			{
				auto textLengh = calculateLineLength(it, itEnd, horizSpaceOffset);
				curX += (sizeX - textLengh);
			}
		}
		else
		{
			//If the current character is a space
			if (ch == ' ')
			{
				//Move over
				curX += (float)space;
			}
			//If the current character is a tab
			else if (ch == '\t')
			{
				//Move over
				curX += (float)tab;
			}
			else
			{
				//create the character vertex
				TextureInfo ti;
				texturePack->get(ch, ti);

				// triangle 1

				// top left
				vertexText.push_back(sf::Vertex(
					{ curX, curY },
					color,
					{ (float)ti.textureRect.left, (float)ti.textureRect.top }
				));

				// top right
				vertexText.push_back(sf::Vertex(
					{ curX + (float)ti.textureRect.width, curY },
					color,
					{ (float)ti.textureRect.left + (float)ti.textureRect.width, (float)ti.textureRect.top }
				));

				// bottom left
				vertexText.push_back(sf::Vertex(
					{ curX, curY + (float)ti.textureRect.height },
					color,
					{ (float)ti.textureRect.left, (float)ti.textureRect.top + (float)ti.textureRect.height }
				));

				// triangle 2

				// top right
				vertexText.push_back(*(vertexText.end() - 2));

				// bottom left
				vertexText.push_back(*(vertexText.end() - 2));

				// bottom right
				vertexText.push_back(sf::Vertex(
					{ curX + (float)ti.textureRect.width, curY + (float)ti.textureRect.height },
					color,
					{ (float)ti.textureRect.left + (float)ti.textureRect.width, (float)ti.textureRect.top + (float)ti.textureRect.height }
				));

				//Move over the width of the character
				curX += (float)ti.textureRect.width;
			}
			if (it < itEnd)
			{
				curX += (float)horizSpaceOffset;
			}
		}
	}
}

void BitmapFont::draw(const VertexArray2& vertexText,
	const sf::Vector2f& pos, const sf::Vector2f& size,
	GameShader* spriteShader, sf::RenderTarget& target) const
{
	vertexText.draw(
		texturePack->getTexture(), pos, size,
		getPalette(), spriteShader, target
	);
}
