#include "LevelHelper.h"
#include "TexturePacks/SimpleIndexedTexturePack.h"
#include "TexturePacks/VectorTexturePack.h"

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
		const std::vector<int16_t>& pillar, CachedImagePack& tileset, bool top)
	{
		size_t i, lim;

		if (top == true)
		{
			// compensate for maps using 5-row min files
			if (pillar.size() == 10)
			{
				y += 3 * 32;
			}
			i = 0;
			lim = pillar.size() - 2;
		}
		else
		{
			i = pillar.size() - 2;
			lim = pillar.size();
		}

		bool notTransparent = false;

		// Each iteration draw one row of the min
		for (; i < lim; i += 2)
		{
			int16_t l = (pillar[i] & 0x0FFF) - 1;
			int16_t r = (pillar[i + 1] & 0x0FFF) - 1;

			notTransparent |= drawMinTile(s, tileset, x, y, l, r);

			y += 32; // down 32 each row
		}
		return notTransparent;
	}

	std::shared_ptr<TexturePack> loadTilesetSprite(
		CachedImagePack& imgPack, const Min& min, bool top, bool skipBlankTiles)
	{
		auto texturePack = std::make_shared<VectorTexturePack>(
			min.size() - 1, imgPack.getPalette(), imgPack.IsIndexed());

		bool hasTile = true;
		sf::Image2 newPillar;

		for (size_t i = 0; i < min.size() - 1; i++)
		{
			if (hasTile == true)
			{
				newPillar.create(64, (top == true ? 256 : 32), sf::Color::Transparent);
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

	std::shared_ptr<TexturePack> loadTilesetSpriteBatchSprites(
		CachedImagePack& imgPack, const Min& min, bool top, bool skipBlankTiles)
	{
		std::shared_ptr<SimpleMultiTexturePack> texturePack;
		SimpleIndexedMultiTexturePack* indexedTexturePack = nullptr;

		if (skipBlankTiles == false)
		{
			texturePack = std::make_shared<SimpleMultiTexturePack>(
				imgPack.getPalette(), imgPack.IsIndexed());
		}
		else
		{
			texturePack = std::make_shared<SimpleIndexedMultiTexturePack>(
				imgPack.getPalette(), imgPack.IsIndexed());
			indexedTexturePack = (SimpleIndexedMultiTexturePack*)texturePack.get();
		}

		sf::Image2 newPillar;
		size_t i = 0;
		size_t xMax = 16;
		size_t yMax = (top == true ? 4 : 32);

		bool mainLoop = true;
		while (mainLoop == true)
		{
			newPillar.create(1024, 1024, sf::Color::Transparent);

			bool loop = true;
			size_t x = 0;
			size_t y = 0;
			while (loop == true)
			{
				size_t newX = x * 64;
				size_t newY = y * (top == true ? 256 : 32);

				bool hasTile = drawMinPillar(newPillar, newX, newY, min[i], imgPack, top);

				if (skipBlankTiles == false ||
					hasTile == true)
				{
					if (indexedTexturePack != nullptr)
					{
						indexedTexturePack->addTextureIndex(i);
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
				if (i >= (min.size() - 1))
				{
					loop = false;
					mainLoop = false;
				}
			}
			texturePack->addTexturePack(std::make_shared<sf::Texture>(newPillar), xMax, yMax, true);
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
			return loadTilesetSpriteBatchSprites(imgPack, min, top, skipBlankTiles);
		}
	}

	void saveTilesetSprite(const std::string& path,
		CachedImagePack& imgPack, const Min& min, int bottomTopOrBoth, bool skipBlankTiles)
	{
		sf::Image2 newPillar;
		size_t i = 0;
		size_t file = 1;
		size_t xMax = 16;
		size_t yMax = (bottomTopOrBoth != 0 ? 4 : 32);

		bool mainLoop = true;
		while (mainLoop == true)
		{
			newPillar.create(1024, 1024, sf::Color::Transparent);

			bool loop = true;
			size_t x = 0;
			size_t y = 0;
			while (loop == true)
			{
				size_t newX = x * 64;
				size_t newY = y * (bottomTopOrBoth != 0 ? 256 : 32);
				bool hasTile = false;

				if (bottomTopOrBoth < 0)
				{
					hasTile = drawMinPillar(newPillar, newX, newY + 224, min[i], imgPack, false);
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
				if (i >= (min.size() - 1))
				{
					loop = false;
					mainLoop = false;
				}
			}
			newPillar.saveToFile(path + std::to_string(file) + ".png");
			file++;
		}
	}
}
