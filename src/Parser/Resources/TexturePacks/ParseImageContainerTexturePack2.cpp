#include "ParseImageContainerTexturePack2.h"
#include "Game/Game.h"
#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
#include "Game/Level/LevelHelper.h"
#include "Resources/CachedImagePack.h"
#include "Resources/ImageContainers/DT1ImageContainer.h"
#endif
#include "Parser/Resources/TexturePacks/ParseImageContainerTexturePack.h"
#include "Parser/Utils/ParseUtils.h"
#include "Resources/TexturePacks/ImageContainerTexturePack2.h"
#include "Resources/TexturePacks/IndexedTexturePack2.h"
#include "Resources/TexturePacks/MultiImageContainerTexturePack2.h"

namespace Parser2
{
	using namespace Parser;
	using namespace rapidjson;
	using namespace std::literals;

#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
	void parseDT1ImageContainerTexturePack(Game& game, const Value& elem,
		const DT1ImageContainer& imgCont, IndexedTexturePack& texturePack)
	{
		const auto& tileIndexes = imgCont.getTileIndexes();

		for (const auto& ti : tileIndexes)
		{
			if (ti.second.size() == 0)
			{
				continue;
			}
			else if (ti.second.size() > 1)
			{
				texturePack.addAnimatedTexture(ti.first, sf::milliseconds(100), ti.second);
			}
			else
			{
				texturePack.mapTextureIndex(ti.first, ti.second.front());
			}
		}
	}

	std::unique_ptr<TexturePack> parseMinImageContainerTexturePack(
		Game& game, const Value& elem, const ImageContainer& imgCont,
		const std::shared_ptr<Palette>& pal, bool useIndexedImages)
	{
		// l4.min and town.min contain 16 blocks, all others 10.
		auto minBlocks = getUIntKey(elem, "minBlocks", 10);
		if (minBlocks != 10 && minBlocks != 16)
		{
			minBlocks = 10;
		}
		Min min(elem["min"sv].GetStringView(), minBlocks);
		if (min.size() == 0)
		{
			return nullptr;
		}
		bool topTiles = getBoolKey(elem, "topTiles", false);
		CachedImagePack imgPack(&imgCont, pal, useIndexedImages);
		return LevelHelper::loadTilesetSprite(imgPack, min, topTiles, topTiles);
	}
#endif

	std::unique_ptr<TexturePack> parseImageContainerTexturePack(Game& game, const Value& elem)
	{
		auto imgContainers = getImageContainers(game, elem);
		if (imgContainers.empty() == true)
		{
			return nullptr;
		}

		std::shared_ptr<Palette> pal;
		if (isValidString(elem, "palette") == true)
		{
			pal = game.Resources().getPalette(elem["palette"sv].GetStringView());
		}

		bool useIndexedImages = pal != nullptr && game.Shaders().hasSpriteShader();

#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
		if (isValidString(elem, "min") == true)
		{
			return parseMinImageContainerTexturePack(
				game, elem, *imgContainers.front(), pal, useIndexedImages
			);
		}
#endif
		auto offset = getVector2fKey<sf::Vector2f>(elem, "offset");
		auto normalizeDirections = getBoolKey(elem, "normalizeDirections");

		if (imgContainers.size() == 1)
		{
			auto texturePack = std::make_unique<ImageContainerTexturePack2>(
				imgContainers.front(), offset, pal, useIndexedImages, normalizeDirections
			);

#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
			auto dt1ImgCont = dynamic_cast<DT1ImageContainer*>(imgContainers.front().get());
			if (dt1ImgCont == nullptr)
			{
				return texturePack;
			}

			auto texturePack2 = std::make_unique<IndexedTexturePack2>(
				std::move(texturePack), true, false);

			parseDT1ImageContainerTexturePack(game, elem, *dt1ImgCont, *texturePack2);

			return texturePack2;
#else
			return texturePack;
#endif
		}
		else
		{
			return std::make_unique<MultiImageContainerTexturePack2>(
				imgContainers, offset, pal, useIndexedImages, normalizeDirections
			);
		}
	}
}
