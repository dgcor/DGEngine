#ifndef NO_DIABLO_FORMAT_SUPPORT
#include "LevelHelper.h"
#include <filesystem>
#include "ImageContainers/CELImageContainer.h"
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
		std::shared_ptr<Palette> palette;
		if (imgPack.IsIndexed() == true)
		{
			palette = imgPack.getPalette();
		}
		auto texturePack = std::make_shared<VectorTexturePack>(min.size(), palette);

		bool hasTile = true;
		sf::Image2 newPillar;
		unsigned pillarHeight = (top == true ? ((min[0].size() - 1) * 32) : 32);
		if (top == true)
		{
			sf::Vector2f offset(0.f, -(float)pillarHeight);
			texturePack->setOffset(offset);
		}

		for (uint32_t i = 0; i < min.size(); i++)
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
		uint32_t minimumTextureSize, uint32_t maximumTextureSize,
		uint32_t numTexturesToFit,
		uint32_t tileWidth, uint32_t tileHeight,
		uint32_t& sheetWidth, uint32_t& sheetHeight,
		uint32_t& numTexturesThatFitX, uint32_t& numTexturesThatFitY)
	{
		numTexturesThatFitX = maximumTextureSize / tileWidth;
		numTexturesThatFitY = maximumTextureSize / tileHeight;
		sheetWidth = tileWidth * numTexturesThatFitX;
		sheetHeight = tileHeight * numTexturesThatFitY;

		while (sheetWidth > minimumTextureSize &&
			sheetHeight > minimumTextureSize)
		{
			uint32_t newSheetWidth, newSheetHeight;
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
			uint32_t newMaxFitX = newSheetWidth / tileWidth;
			uint32_t newMaxFitY = newSheetHeight / tileHeight;
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
		uint32_t numTexturesToFit;

		std::shared_ptr<Palette> palette;
		if (imgPack.IsIndexed() == true)
		{
			palette = imgPack.getPalette();
		}

		if (skipBlankTiles == false)
		{
			texturePack = std::make_shared<SimpleMultiTexturePack>(palette);
			multiTexturePack = (SimpleMultiTexturePack*)texturePack.get();
			numTexturesToFit = min.size();
		}
		else
		{
			auto texturePack2 = std::make_unique<SimpleMultiTexturePack>(palette);
			multiTexturePack = texturePack2.get();
			texturePack = std::make_shared<IndexedTexturePack>(std::move(texturePack2), true);
			indexedTexturePack = (IndexedTexturePack*)texturePack.get();
			numTexturesToFit = min.size() - min.blankTopPillars();
		}

		uint32_t maxTextureSize = sf::Texture::getMaximumSize();
		if (maxTextureSize > 1024u)
		{
			// with this maximum, all original pillars fit into one texture.
			maxTextureSize = std::min(maxTextureSize, 2560u);
		}
		uint32_t pillarWidth = 64;
		uint32_t pillarHeight = (top == true ? ((min[0].size() - 1) * 32) : 32);
		if (pillarHeight > maxTextureSize)
		{
			return texturePack;
		}
		uint32_t i = 0;
		uint32_t xMax, yMax, sheetWidth, sheetHeight;

		getIdealTilesetSheetSize(1024, maxTextureSize, numTexturesToFit,
			pillarWidth, pillarHeight, sheetWidth, sheetHeight, xMax, yMax);

		sf::Vector2f offset(0.f, (top == true ? -(float)pillarHeight : 0.f));

		bool mainLoop = true;
		while (mainLoop == true)
		{
			sf::Image2 newPillar;
			newPillar.create(sheetWidth, sheetHeight, sf::Color::Transparent);

			bool loop = true;
			uint32_t x = 0;
			uint32_t y = 0;
			while (loop == true)
			{
				uint32_t newX = x * pillarWidth;
				uint32_t newY = y * pillarHeight;

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
				std::make_pair(xMax, yMax), offset, 0, 0, true, AnimationType::Looped);

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

	void saveTilesetSprite(const std::string& outFilePath, const std::string& outFileName,
		CachedImagePack& imgPack, const Min& min, int bottomTopOrBoth,
		bool skipBlankTiles, unsigned maxTextureSize)
	{
		try
		{
			std::filesystem::path outPath(outFilePath);

			if (std::filesystem::exists(outPath) == false ||
				std::filesystem::is_directory(outPath) == false)
			{
				return;
			}

			uint32_t numTexturesToFit = min.size();
			if (bottomTopOrBoth == 1 &&
				skipBlankTiles == true)
			{
				numTexturesToFit -= min.blankTopPillars();
			}
			uint32_t pillarWidth = 64;
			uint32_t pillarHeight = (bottomTopOrBoth != 0 ? (min[0].size() * 32) : 32);
			if (pillarHeight > maxTextureSize)
			{
				return;
			}
			uint32_t i = 0;
			uint32_t xMax, yMax, sheetWidth, sheetHeight;

			getIdealTilesetSheetSize(1024, maxTextureSize, numTexturesToFit,
				pillarWidth, pillarHeight, sheetWidth, sheetHeight, xMax, yMax);

			uint32_t file = 1;

			bool mainLoop = true;
			while (mainLoop == true)
			{
				sf::Image2 newPillar;
				newPillar.create(sheetWidth, sheetHeight, sf::Color::Transparent);

				bool loop = true;
				uint32_t x = 0;
				uint32_t y = 0;
				while (loop == true)
				{
					uint32_t newX = x * pillarWidth;
					uint32_t newY = y * pillarHeight;
					bool hasTile = false;

					if (bottomTopOrBoth < 0)
					{
						hasTile = drawMinPillar(
							newPillar, newX, newY + pillarHeight - 32, min[i], imgPack, false
						);
						hasTile |= drawMinPillar(
							newPillar, newX, newY, min[i], imgPack, true
						);
					}
					else
					{
						hasTile = drawMinPillar(
							newPillar, newX, newY, min[i], imgPack, bottomTopOrBoth != 0
						);
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

				auto newFilePath = outPath;
				if (file > 1 || mainLoop == true)
				{
					newFilePath /= (outFileName + Utils::toString(file) + ".png");
				}
				else
				{
					newFilePath /= (outFileName + ".png");
				}
				newPillar.saveToFile(newFilePath.string());
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
		catch (std::exception&) {}
	}

	void loadAndSaveTilesetSprite(const std::string& celFile,
		const std::string& palFile, Palette::ColorFormat colorFormat, const std::string& outFilePath,
		int bottomTopOrBoth, bool skipBlankTiles, unsigned maxTextureSize, unsigned minBlock)
	{
		try
		{
			std::filesystem::path celPath(celFile);
			std::filesystem::path outPath(outFilePath);

			if (std::filesystem::is_directory(outPath) == false ||
				celPath.has_filename() == false)
			{
				return;
			}

			bool toUpper = false;
			if (celPath.has_extension() && celPath.extension() != ".cel")
			{
				toUpper = true;
			}

			auto palPath = palFile.empty() == true ? celPath : std::filesystem::path(palFile);
			auto minPath = celPath;

			if (toUpper == false)
			{
				if (palFile.empty() == true)
				{
					palPath.replace_extension(".pal");
				}
				minPath.replace_extension(".min");
			}
			else
			{
				if (palFile.empty() == true)
				{
					palPath.replace_extension(".PAL");
				}
				minPath.replace_extension(".MIN");
			}

			auto pal = std::make_shared<Palette>(palPath.string(), colorFormat);
			CELImageContainer celImgContainer(celPath.string());
			CachedImagePack imgPack(&celImgContainer, pal, false);
			Min min(minPath.string(), minBlock);

			saveTilesetSprite(
				outPath.string(), celPath.stem().string(), imgPack,
				min, bottomTopOrBoth, skipBlankTiles, maxTextureSize
			);
		}
		catch (std::exception&) {}
	}
}
#endif
