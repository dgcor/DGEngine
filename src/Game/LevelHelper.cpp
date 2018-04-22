#include "LevelHelper.h"
#include "TexturePacks/IndexedTexturePack.h"
#include "TexturePacks/SimpleTexturePack.h"
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
			sf::Vector2f offset(0.f, -32.f);
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

	std::shared_ptr<TexturePack> loadTilesetAndBatchSprites(
		CachedImagePack& imgPack, const Min& min, bool top, bool skipBlankTiles)
	{
		std::shared_ptr<TexturePack> texturePack;
		SimpleMultiTexturePack* multiTexturePack = nullptr;
		IndexedTexturePack* indexedTexturePack = nullptr;

		if (skipBlankTiles == false)
		{
			texturePack = std::make_shared<SimpleMultiTexturePack>(
				imgPack.getPalette(), imgPack.IsIndexed());
			multiTexturePack = (SimpleMultiTexturePack*)texturePack.get();
		}
		else
		{
			auto texturePack2 = std::make_unique<SimpleMultiTexturePack>(
				imgPack.getPalette(), imgPack.IsIndexed());
			multiTexturePack = texturePack2.get();
			texturePack = std::make_shared<IndexedTexturePack>(std::move(texturePack2), true);
			indexedTexturePack = (IndexedTexturePack*)texturePack.get();
		}

		unsigned pillarHeight = (top == true ? ((min[0].size() - 1) * 32) : 32);
		if (pillarHeight > 1024)
		{
			return texturePack;
		}
		size_t i = 0;
		size_t xMax = 16;
		size_t yMax = (top == true ? (1024 / pillarHeight) : 32);
		unsigned sheetHeight = pillarHeight * yMax;
		sf::Vector2f offset(0.f, (top == true ? -32.f : 0.f));
		sf::Image2 newPillar;

		bool mainLoop = true;
		while (mainLoop == true)
		{
			newPillar.create(1024, sheetHeight, sf::Color::Transparent);

			bool loop = true;
			size_t x = 0;
			size_t y = 0;
			while (loop == true)
			{
				size_t newX = x * 64;
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

	void saveTilesetSprite(const std::string& path,
		CachedImagePack& imgPack, const Min& min, int bottomTopOrBoth, bool skipBlankTiles)
	{
		sf::Image2 newPillar;
		size_t i = 0;
		size_t file = 1;
		size_t xMax = 16;
		size_t yMax = (bottomTopOrBoth != 0 ? 4 : 32);
		size_t topPillarHeight = (min[0].size() - 1) * 32;

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
					hasTile = drawMinPillar(newPillar, newX, newY + topPillarHeight, min[i], imgPack, false);
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
			newPillar.saveToFile(path + std::to_string(file) + ".png");
			file++;
		}
	}
}
