#pragma once

#include <memory>
#include "Resources/CachedImagePack.h"
#include "Resources/Min.h"
#include "Resources/Palette.h"
#include "Resources/TexturePack.h"
#include <string>

namespace LevelHelper
{
	std::unique_ptr<TexturePack> loadTilesetSprite(CachedImagePack& imgPack,
		const Min& min, bool top, bool skipBlankTiles);

	// bottomTopOrBoth : -1 for both, 0 for bottom, 1 for top
	void saveTilesetSprite(const std::string& outFilePath, const std::string& outFileName,
		CachedImagePack& imgPack, const Min& min, int bottomTopOrBoth,
		bool skipBlankTiles, unsigned maxTextureSize = 1024u);

	// bottomTopOrBoth : -1 for both, 0 for bottom, 1 for top
	void loadAndSaveTilesetSprite(const std::string& celFile,
		const std::string& palFile, Palette::ColorFormat colorFormat, const std::string& outFilePath,
		int bottomTopOrBoth, bool skipBlankTiles, unsigned maxTextureSize, unsigned minBlock);
}
