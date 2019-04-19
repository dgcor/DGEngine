#include "BitmapFontTexturePack.h"
#include "TextureInfo.h"

// This piece of code was originally from Lazy Foo' Productions (http://lazyfoo.net/)
void BitmapFontTexturePack::calculateCharSizes(const sf::Image& img, bool verticalDirection)
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
	chars[' '].width = cellW / 2;

	//Calculate new line
	chars['\n'].width = baseA - top;

	//Lop off excess top pixels
	for (auto& chrRect : chars)
	{
		chrRect.top += top;
		chrRect.height -= top;
	}
}

BitmapFontTexturePack::BitmapFontTexturePack(const std::shared_ptr<sf::Texture>& tex, int rows_,
	int columns_, bool verticalDirection, const sf::Image& img)
	: texture(tex), rows(rows_), columns(columns_)
{
	this->calculateCharSizes(img, verticalDirection);
}

BitmapFontTexturePack::BitmapFontTexturePack(const std::shared_ptr<sf::Texture>& tex, int rows_,
	int columns_, bool verticalDirection, const std::vector<uint8_t>& charSizes)
	: texture(tex), rows(rows_), columns(columns_)
{
	size_t charStartIdx = 0;
	if (charSizes.size() == 130 || charSizes.size() == 258)
	{
		charStartIdx = 2;
	}
	else if ((charSizes.size() == 128 || charSizes.size() == 256) == false)
	{
		this->calculateCharSizes(tex->copyToImage(), verticalDirection);
		return;
	}

	int cellW = tex->getSize().x / columns;
	int cellH = tex->getSize().y / rows;
	int iRow = 0;
	int iCol = 0;
	for (size_t i = 0; i < 256; i++)
	{
		chars[i].left = cellW * iCol;
		chars[i].top = cellH * iRow;
		auto charSizeIdx = i + charStartIdx;
		if (charSizeIdx < charSizes.size())
		{
			chars[i].width = charSizes[charSizeIdx];
		}
		else
		{
			chars[i].width = 0;
		}
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

	if (charStartIdx > 0)
	{
		chars[' '].width = charSizes[0];
		chars['\n'].width = charSizes[1];
	}
	else
	{
		chars[' '].width = charSizes[' '];
		chars['\n'].width = charSizes['\n'];
	}
}

bool BitmapFontTexturePack::get(size_t index, TextureInfo& ti) const
{
	if (index < chars.size())
	{
		ti.texture = texture.get();
		ti.textureRect = chars[index];
		ti.offset = {};
		ti.absoluteOffset = false;
		ti.palette = palette;
		return true;
	}
	return false;
}
