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

Palette* BitmapFont::getPalette() const noexcept
{
	if (palette != nullptr)
	{
		return palette.get();
	}
	else if (charTextures->getPalette() != nullptr)
	{
		return charTextures->getPalette().get();
	}
	return nullptr;
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

void BitmapFont::updateVertexString(std::vector<sf::Vertex>& vertexText,
	const std::string& text, sf::Color color, int horizSpaceOffset,
	int vertSpaceOffset, float sizeX, HorizontalAlign align) const
{
	vertexText.clear();
	vertexText.resize(text.size() * 6);

	if (hasPalette() == true)
	{
		color = sf::Color::White;
	}
	else if (color == sf::Color::White)
	{
		color = defaultColor;
	}

	//Temp offsets
	float curX = 0.f;
	float curY = 0.f;
	bool wasSpace = false;

	if (align == HorizontalAlign::Center)
	{
		curX += std::round((sizeX / 2.f) - (calculateLineLength(text.data(), horizSpaceOffset) / 2.f));
	}
	else if (align == HorizontalAlign::Right)
	{
		curX += (sizeX - calculateLineLength(text.data(), horizSpaceOffset));
	}

	size_t vertIdx = 0;

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
			curX = 0;
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
				//create the character vertex
				TextureInfo ti;
				charTextures->get((size_t)ch, ti);

				// triangle 1

				// top left
				vertexText[vertIdx].position.x = curX;
				vertexText[vertIdx].position.y = curY;
				vertexText[vertIdx].color = color;
				vertexText[vertIdx].texCoords.x = (float)ti.textureRect.left;
				vertexText[vertIdx].texCoords.y = (float)ti.textureRect.top;
				vertIdx++;

				// top right
				vertexText[vertIdx].position.x = curX + (float)ti.textureRect.width;
				vertexText[vertIdx].position.y = curY;
				vertexText[vertIdx].color = color;
				vertexText[vertIdx].texCoords.x = (float)ti.textureRect.left + (float)ti.textureRect.width;
				vertexText[vertIdx].texCoords.y = (float)ti.textureRect.top;
				vertIdx++;

				// bottom left
				vertexText[vertIdx].position.x = curX;
				vertexText[vertIdx].position.y = curY + (float)ti.textureRect.height;
				vertexText[vertIdx].color = color;
				vertexText[vertIdx].texCoords.x = (float)ti.textureRect.left;
				vertexText[vertIdx].texCoords.y = (float)ti.textureRect.top + (float)ti.textureRect.height;
				vertIdx++;

				// triangle 2

				// top right
				vertexText[vertIdx] = vertexText[vertIdx - 2];
				vertIdx++;

				// bottom left
				vertexText[vertIdx] = vertexText[vertIdx - 2];
				vertIdx++;

				// bottom right
				vertexText[vertIdx].position.x = curX + (float)ti.textureRect.width;
				vertexText[vertIdx].position.y = curY + (float)ti.textureRect.height;
				vertexText[vertIdx].color = color;
				vertexText[vertIdx].texCoords.x = (float)ti.textureRect.left + (float)ti.textureRect.width;
				vertexText[vertIdx].texCoords.y = (float)ti.textureRect.top + (float)ti.textureRect.height;
				vertIdx++;

				//Move over the width of the character + padding
				curX += (float)ti.textureRect.width + (float)(wasSpace ? 0 : padding);
				wasSpace = false;
			}
			curX += (float)horizSpaceOffset;
		}
	}

	vertexText.resize(vertIdx);
}

void BitmapFont::draw(const std::vector<sf::Vertex>& vertexText,
	const sf::Vector2f& pos, const sf::Vector2f& size,
	const Game& game, sf::RenderTarget& target) const
{
	sf::Transform t;
	t.transformRect({ {}, size });
	t = t.translate(pos);

	sf::Shader* spriteShader = nullptr;
	auto currPal = getPalette();
	if (currPal != nullptr)
	{
		spriteShader = game.Shaders().Sprite;
	}

	sf::RenderStates states(sf::BlendAlpha, t, &charTextures->getTexture(), spriteShader);

	if (spriteShader != nullptr)
	{
		spriteShader->setUniform("pixelSize", sf::Glsl::Vec2(
			1.0f / (float)size.x,
			1.0f / (float)size.y
		));
		spriteShader->setUniform("outline", sf::Glsl::Vec4(sf::Color::Transparent));
		spriteShader->setUniform("ignore", sf::Glsl::Vec4(sf::Color::Transparent));
		spriteShader->setUniform("light", sf::Glsl::Vec4(sf::Color::Transparent));
		spriteShader->setUniform("hasPalette", currPal != nullptr);
		if (currPal != nullptr)
		{
			spriteShader->setUniform("palette", currPal->texture);
		}
	}
	if (vertexText.empty() == false)
	{
		target.draw(vertexText.data(), vertexText.size(), sf::PrimitiveType::Triangles, states);
	}
}
