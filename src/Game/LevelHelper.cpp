#include "LevelHelper.h"
#include "TexturePacks/IndexedTexturePack.h"
#include "TexturePacks/SimpleTexturePack.h"
#include "TexturePacks/VectorTexturePack.h"
#include "Utils/Utils.h"

namespace LevelHelper
{
	void drawFrame(sf::Image& s, int start_x, int start_y, const sf::Image& frame)
	{
		auto size = s.getSize();
		auto frameSize = frame.getSize();

		for (size_t x = 0; x < frameSize.x; x++)
		{
			for (size_t y = 0; y < frameSize.y; y++)
			{
				size_t xx = start_x + x;
				size_t yy = start_y + y;

				if (xx < size.x && yy < size.y)
				{
					s.setPixel(xx, yy, frame.getPixel(x, y));
				}
			}
		}
	}

	bool drawMinTile(sf::Image& s, CachedImagePack& f, int x, int y, int16_t l, int16_t r)
	{
		bool notTransparent = false;
		if (l != -1)
		{
			drawFrame(s, x, y, f[l]);
			notTransparent = true;
		}
		if (r != -1)
		{
			drawFrame(s, x + 32, y, f[r]);
			notTransparent = true;
		}
		return notTransparent;
	}

	bool drawMinPillar(sf::Image& s, int x, int y,
		const MinPillar& pillar,
		CachedImagePack& tileset, bool top)
	{
		size_t i, size;

		if (top == true)
		{
			i = 0;
			size = pillar.size() - 1;
		}
		else
		{
			i = pillar.size() - 1;
			size = pillar.size();
		}

		bool notTransparent = false;

		// Each iteration draw one row of the min
		for (; i < size; i++)
		{
			int16_t l = pillar.getLeftTile(i);
			int16_t r = pillar.getRightTile(i);

			notTransparent |= drawMinTile(s, tileset, x, y, l, r);

			y += 32; // down 32 each row
		}
		return notTransparent;
	}

	std::shared_ptr<TexturePack> loadTilesetSprite(
		CachedImagePack& imgPack, const Min& min, bool top, bool skipBlankTiles)
	{
		auto texturePack = std::make_shared<VectorTexturePack>(
			min.size(), imgPack.getPalette(), imgPack.IsIndexed());

		bool hasTile = true;
		sf::Image2 newPillar;
		unsigned pillarHeight = (top == true ? ((min[0].size() - 1) * 32) : 32);
		if (top == true)
		{
			sf::Vector2f offset(0.f, -(float)pillarHeight);
			texturePack->setOffset(offset);
		}

		for (size_t i = 0; i < min.size(); i++)
		{
			if (hasTile == true)
			{
				newPillar.create(64, pillarHeight, sf::Color::Transparent);
				hasTile = false;
			}

			hasTile = drawMinPillar(newPillar, 0, 0, min[i], imgPack, top);

			if (skipBlankTiles == false ||
				hasTile == true)
			{
				texturePack->set(i, newPillar);
			}
		}
		return texturePack;
	}

	// reduces the sheet size and number of textures that fit in a texture
	// to avoid creating big textures with too many blanks.
	void getIdealTilesetSheetSize(
		size_t minimumTextureSize, size_t maximumTextureSize,
		size_t numTexturesToFit,
		size_t tileWidth, size_t tileHeight,
		size_t& sheetWidth, size_t& sheetHeight,
		size_t& numTexturesThatFitX, size_t& numTexturesThatFitY)
	{
		numTexturesThatFitX = maximumTextureSize / tileWidth;
		numTexturesThatFitY = maximumTextureSize / tileHeight;
		sheetWidth = tileWidth * numTexturesThatFitX;
		sheetHeight = tileHeight * numTexturesThatFitY;

		while (sheetWidth > minimumTextureSize &&
			sheetHeight > minimumTextureSize)
		{
			size_t newSheetWidth, newSheetHeight;
			if (sheetWidth >= sheetHeight)
			{
				newSheetWidth = sheetWidth - tileWidth;
				newSheetHeight = sheetHeight;
			}
			else
			{
				newSheetWidth = sheetWidth;
				newSheetHeight = sheetHeight - tileHeight;
			}
			size_t newMaxFitX = newSheetWidth / tileWidth;
			size_t newMaxFitY = newSheetHeight / tileHeight;
			auto maxNumTextures = newMaxFitX * newMaxFitY;
			if (maxNumTextures < numTexturesToFit)
			{
				return;
			}
			numTexturesThatFitX = newMaxFitX;
			numTexturesThatFitY = newMaxFitY;
			sheetWidth = newSheetWidth;
			sheetHeight = newSheetHeight;
		}
	}

	// creates spritesheets for level sprites based on the maximum supported texture size
	// and tries to fit as many tiles into the smallest number of textures.
	std::shared_ptr<TexturePack> loadTilesetAndBatchSprites(
		CachedImagePack& imgPack, const Min& min, bool top, bool skipBlankTiles)
	{
		std::shared_ptr<TexturePack> texturePack;
		SimpleMultiTexturePack* multiTexturePack = nullptr;
		IndexedTexturePack* indexedTexturePack = nullptr;
		size_t numTexturesToFit;

		if (skipBlankTiles == false)
		{
			texturePack = std::make_shared<SimpleMultiTexturePack>(
				imgPack.getPalette(), imgPack.IsIndexed());
			multiTexturePack = (SimpleMultiTexturePack*)texturePack.get();
			numTexturesToFit = min.size();
		}
		else
		{
			auto texturePack2 = std::make_unique<SimpleMultiTexturePack>(
				imgPack.getPalette(), imgPack.IsIndexed());
			multiTexturePack = texturePack2.get();
			texturePack = std::make_shared<IndexedTexturePack>(std::move(texturePack2), true);
			indexedTexturePack = (IndexedTexturePack*)texturePack.get();
			numTexturesToFit = min.size() - min.blankTopPillars();
		}

		auto maxTextureSize = sf::Texture::getMaximumSize();
		if (maxTextureSize > 1024u)
		{
			// with this maximum, all original pillars fit into one texture.
			maxTextureSize = std::min(maxTextureSize, 2560u);
		}
		unsigned pillarWidth = 64;
		unsigned pillarHeight = (top == true ? ((min[0].size() - 1) * 32) : 32);
		if (pillarHeight > maxTextureSize)
		{
			return texturePack;
		}
		size_t i = 0;
		size_t xMax, yMax, sheetWidth, sheetHeight;

		getIdealTilesetSheetSize(1024, maxTextureSize, numTexturesToFit,
			pillarWidth, pillarHeight, sheetWidth, sheetHeight, xMax, yMax);

		sf::Vector2f offset(0.f, (top == true ? -(float)pillarHeight : 0.f));

		bool mainLoop = true;
		while (mainLoop == true)
		{
			sf::Image2 newPillar;
			newPillar.create(sheetWidth, sheetHeight, sf::Color::Transparent);

			bool loop = true;
			size_t x = 0;
			size_t y = 0;
			while (loop == true)
			{
				size_t newX = x * pillarWidth;
				size_t newY = y * pillarHeight;

				bool hasTile = drawMinPillar(newPillar, newX, newY, min[i], imgPack, top);

				if (skipBlankTiles == false ||
					hasTile == true)
				{
					if (indexedTexturePack != nullptr)
					{
						indexedTexturePack->mapTextureIndex(i);
					}

					x++;
					if (x >= xMax)
					{
						x = 0;
						y++;
						if (y >= yMax)
						{
							loop = false;
						}
					}
				}

				i++;
				if (i >= min.size())
				{
					loop = false;
					mainLoop = false;
				}
			}
			multiTexturePack->addTexturePack(std::make_shared<sf::Texture>(newPillar),
				std::make_pair(xMax, yMax), offset, 0, true);

			// calculate new sheet size for the remaining tiles.
			if (mainLoop == true)
			{
				numTexturesToFit -= xMax * yMax;
				getIdealTilesetSheetSize(std::min(512u, maxTextureSize),
					maxTextureSize, numTexturesToFit,
					pillarWidth, pillarHeight, sheetWidth, sheetHeight, xMax, yMax);
			}
		}
		return texturePack;
	}

	std::shared_ptr<TexturePack> loadTilesetSprite(CachedImagePack& imgPack,
		const Min& min, bool top, bool skipBlankTiles, bool batchSpritesTogether)
	{
		if (batchSpritesTogether == false)
		{
			return loadTilesetSprite(imgPack, min, top, skipBlankTiles);
		}
		else
		{
			return loadTilesetAndBatchSprites(imgPack, min, top, skipBlankTiles);
		}
	}

	void saveTilesetSprite(const std::string& path, CachedImagePack& imgPack,
		const Min& min, int bottomTopOrBoth, bool skipBlankTiles, unsigned maxTextureSize)
	{
		size_t numTexturesToFit = min.size();
		if (bottomTopOrBoth == 1 &&
			skipBlankTiles == true)
		{
			numTexturesToFit -= min.blankTopPillars();
		}
		unsigned pillarWidth = 64;
		unsigned pillarHeight = (bottomTopOrBoth != 0 ? (min[0].size() * 32) : 32);
		if (pillarHeight > maxTextureSize)
		{
			return;
		}
		size_t i = 0;
		size_t xMax, yMax, sheetWidth, sheetHeight;

		getIdealTilesetSheetSize(1024, maxTextureSize, numTexturesToFit,
			pillarWidth, pillarHeight, sheetWidth, sheetHeight, xMax, yMax);

		size_t file = 1;

		bool mainLoop = true;
		while (mainLoop == true)
		{
			sf::Image2 newPillar;
			newPillar.create(sheetWidth, sheetHeight, sf::Color::Transparent);

			bool loop = true;
			size_t x = 0;
			size_t y = 0;
			while (loop == true)
			{
				size_t newX = x * pillarWidth;
				size_t newY = y * pillarHeight;
				bool hasTile = false;

				if (bottomTopOrBoth < 0)
				{
					hasTile = drawMinPillar(newPillar, newX, newY + pillarHeight - 32, min[i], imgPack, false);
					hasTile |= drawMinPillar(newPillar, newX, newY, min[i], imgPack, true);
				}
				else
				{
					hasTile = drawMinPillar(newPillar, newX, newY, min[i], imgPack, bottomTopOrBoth != 0);
				}

				if (skipBlankTiles == false ||
					hasTile == true)
				{
					x++;
					if (x >= xMax)
					{
						x = 0;
						y++;
						if (y >= yMax)
						{
							loop = false;
						}
					}
				}

				i++;
				if (i >= min.size())
				{
					loop = false;
					mainLoop = false;
				}
			}
			newPillar.saveToFile(path + Utils::toString(file) + ".png");
			file++;

			// calculate new sheet size for the remaining tiles.
			if (mainLoop == true)
			{
				numTexturesToFit -= xMax * yMax;
				getIdealTilesetSheetSize(std::min(512u, maxTextureSize),
					maxTextureSize, numTexturesToFit,
					pillarWidth, pillarHeight, sheetWidth, sheetHeight, xMax, yMax);
			}
		}
	}
}
