#include "BitmapFontTexturePack.h"

// This piece of code was originally from Lazy Foo' Productions (http://lazyfoo.net/)
void BitmapFontTexturePack::calculateCharSizes(const sf::Image& img, int rows, int columns,
	int16_t newLine, int16_t space, int16_t tab, bool verticalDirection)
{
	//Set the cell dimensions
	int cellW = img.getSize().x / columns;
	int cellH = img.getSize().y / rows;

	charRects.resize(rows * columns);

	//New line variables
	int top = cellH;
	int baseA = cellH;

	//Go through the cell rows
	int iRow = 0;
	int iCol = 0;
	for (auto& charRect : charRects)
	{
		//Set the character offset
		charRect.left = cellW * iCol;
		charRect.top = cellH * iRow;

		//Set the dimensions of the character
		charRect.width = cellW;
		charRect.height = cellH;

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
				if (img.getPixel(pX, pY).a != 0)
				{
					//Set the x offset
					charRect.left = pX;

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
				if (img.getPixel(pX, pY).a != 0)
				{
					//Set the width
					charRect.width = (pX - charRect.left) + 1;

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
				if (img.getPixel(pX, pY).a != 0)
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

	//Calculate new line
	if (newLine < 0 && charRects.size() > 'A')
	{
		//Find Bottom of A
		auto& charRect = charRects['A'];

		//Go through pixel rows
		for (int pRow = charRect.height - 1; pRow >= 0; pRow--)
		{
			//Go through pixel columns
			for (int pCol = 0; pCol < charRect.width; pCol++)
			{
				//Get the pixel offsets
				int pX = charRect.left + pCol;
				int pY = charRect.top + pRow;

				//If a non colorkey pixel is found
				if (img.getPixel(pX, pY).a != 0)
				{
					//Bottom of a is found
					baseA = pRow;

					//Break the loops
					pCol = charRect.width;
					pRow = -1;
				}
			}
		}
		charRects['\n'].width = baseA - top;
	}
	else if (newLine > 0)
	{
		charRects['\n'].width = newLine;
	}

	//Calculate space
	if (space < 0)
	{
		charRects[' '].width = cellW / 2;
	}
	else if (space > 0)
	{
		charRects[' '].width = space;
	}

	//Calculate tab
	if (tab < 0)
	{
		charRects['\t'].width = cellW * 2;
	}
	else if (space > 0)
	{
		charRects['\t'].width = tab;
	}

	//Loop off excess top pixels
	for (auto& charRect : charRects)
	{
		charRect.top += top;
		charRect.height -= top;
	}
}

BitmapFontTexturePack::BitmapFontTexturePack(const std::shared_ptr<sf::Texture>& tex,
	const std::shared_ptr<Palette>& palette_, int rows, int columns,
	int16_t newLine, int16_t space, int16_t tab, bool verticalDirection)
	: texture(tex), palette(palette_)
{
	calculateCharSizes(tex->copyToImage(), rows, columns, newLine, space, tab, verticalDirection);
}

BitmapFontTexturePack::BitmapFontTexturePack(const std::shared_ptr<sf::Texture>& tex,
	const std::shared_ptr<Palette>& palette_, int rows, int columns,
	int16_t newLine, int16_t space, int16_t tab, bool verticalDirection,
	const std::vector<uint8_t>& charSizes, size_t startPos, size_t skipNBytes)
	: texture(tex), palette(palette_)
{
	size_t charStartIdx = 0;
	if (charSizes.size() == 130 || charSizes.size() == 258)
	{
		charStartIdx = 2;
	}
	else if ((charSizes.size() == 128 || charSizes.size() == 256) == false &&
		charSizes.size() <= 256)
	{
		calculateCharSizes(tex->copyToImage(), rows, columns, newLine, space, tab, verticalDirection);
		return;
	}
	else
	{
		charStartIdx = startPos;
	}
	if (skipNBytes == 0)
	{
		skipNBytes = 1;
	}

	charRects.resize(rows * columns);

	int cellW = tex->getSize().x / columns;
	int cellH = tex->getSize().y / rows;
	int iRow = 0;
	int iCol = 0;
	for (size_t i = 0; i < 256; i++)
	{
		charRects[i].left = cellW * iCol;
		charRects[i].top = cellH * iRow;
		auto charSizeIdx = (i * skipNBytes) + charStartIdx;
		if (charSizeIdx < charSizes.size())
		{
			charRects[i].width = charSizes[charSizeIdx];
		}
		else
		{
			charRects[i].width = 0;
		}
		charRects[i].height = cellH;

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


	if (newLine > 0)
	{
		charRects['\n'].width = newLine;
	}
	else if (charStartIdx == 2 &&
		(charSizes.size() == 130 || charSizes.size() == 258))
	{
		charRects['\n'].width = charSizes[1];
	}

	if (space > 0)
	{
		charRects[' '].width = space;
	}
	else if (charStartIdx == 2 &&
		(charSizes.size() == 130 || charSizes.size() == 258))
	{
		charRects[' '].width = charSizes[0];
	}

	if (tab > 0)
	{
		charRects['\t'].width = tab;
	}
}

bool BitmapFontTexturePack::get(uint32_t index, TextureInfo& ti) const
{
	if (index < charRects.size())
	{
		ti.texture = texture.get();
		ti.textureRect = charRects[index];
		ti.palette = palette;
		ti.offset = {};
		ti.absoluteOffset = false;
		ti.blendMode = BlendMode::Alpha;
		ti.nextIndex = -1;
		return true;
	}
	return false;
}

int32_t BitmapFontTexturePack::getWidth(uint32_t index) const noexcept
{
	return index < charRects.size() ? charRects[index].width : 0;
}
