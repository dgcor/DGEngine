#pragma once

#include "CachedImagePack.h"
#include <memory>
#include "Min.h"
#include <string>
#include "TexturePacks/TexturePack.h"

namespace LevelHelper
{
	std::shared_ptr<TexturePack> loadTilesetSprite(CachedImagePack& imgPack,
		const Min& min, bool top, bool skipBlankTiles, bool batchSpritesTogether);

	// bottomTopOrBoth : -1 for both, 0 for bottom, 1 for top
	void saveTilesetSprite(const std::string& outFilePath, const std::string& outFileName,
		CachedImagePack& imgPack, const Min& min, int bottomTopOrBoth,
		bool skipBlankTiles, unsigned maxTextureSize = 1024u);

	// bottomTopOrBoth : -1 for both, 0 for bottom, 1 for top
	void loadAndSaveTilesetSprite(const std::string& celFile,
		const std::string& palFile, const std::string& outFilePath,
		int bottomTopOrBoth, bool skipBlankTiles, unsigned maxTextureSize, unsigned minBlock);
}
