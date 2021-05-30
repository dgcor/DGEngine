#include "ParseTexturePack2.h"
#include "FileUtils.h"
#include "Game.h"
#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
#include "CachedImagePack.h"
#include "Game/LevelHelper.h"
#include "ImageContainers/DT1ImageContainer.h"
#endif
#include "Parser/ParseTexturePack.h"
#include "Parser/Utils/ParseUtils.h"
#include <SFML/System/Utf.hpp>
#include "TexturePacks/BitmapFontTexturePack.h"
#include "TexturePacks/CachedTexturePack2.h"
#include "TexturePacks/CompositeTexturePack2.h"
#include "TexturePacks/IndexedTexturePack2.h"
#include "TexturePacks/RectTexturePack.h"
#include "TexturePacks/SimpleTexturePack.h"
#include "TexturePacks/StackedTexturePack2.h"
#include "Utils/NumberVector.h"

namespace Parser2
{
	using namespace Parser;
	using namespace rapidjson;
	using namespace std::literals;

	enum class TexturePackType
	{
		Generic,
		BitmapFont,
		Stacked,
		Composite
	};

	TexturePackType getTexturePackType(const Value& elem)
	{
		auto fileType = getStringViewKey(elem, "type");
		if (fileType == "font")
		{
			return TexturePackType::BitmapFont;
		}
		else if (fileType == "composite")
		{
			return TexturePackType::Composite;
		}
		if (elem.HasMember("texturePacks"sv) == true)
		{
#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
			if (elem.HasMember("file"sv) == true)
			{
				return TexturePackType::Composite;
			}
#endif
			return TexturePackType::Stacked;
		}
		return TexturePackType::Generic;
	}

#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
	void parseDT1ImageContainerTexturePack(Game& game, const Value& elem,
		const DT1ImageContainer& imgCont, IndexedTexturePack2& texturePack)
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

	std::unique_ptr<TexturePack> parseImageContainerTexturePack(
		Game& game, const Value& elem)
	{
		std::vector<std::shared_ptr<ImageContainer>> imgVec;

		const auto& imgElem = elem["imageContainer"sv];
		if (imgElem.IsString() == true)
		{
			auto imgCont = game.Resources().getImageContainer(imgElem.GetStringView());
			if (imgCont != nullptr)
			{
				imgVec.push_back(imgCont);
			}
		}
		else if (imgElem.IsArray() == true)
		{
			for (const auto& val : imgElem)
			{
				auto imgCont = game.Resources().getImageContainer(getStringViewVal(val));
				if (imgCont != nullptr)
				{
					imgVec.push_back(imgCont);
				}
			}
		}
		if (imgVec.empty() == true)
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
				game, elem, *imgVec.front(), pal, useIndexedImages
			);
		}
#endif
		auto offset = getVector2fKey<sf::Vector2f>(elem, "offset");
		auto normalizeDirections = getBoolKey(elem, "normalizeDirections");

		if (imgVec.size() == 1)
		{
			auto texturePack = std::make_unique<CachedTexturePack2>(
				imgVec.front(), offset, pal, useIndexedImages, normalizeDirections
			);

#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
			auto dt1ImgCont = dynamic_cast<DT1ImageContainer*>(imgVec.front().get());
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
			return std::make_unique<CachedMultiTexturePack2>(
				imgVec, offset, pal, useIndexedImages, normalizeDirections
			);
		}
	}

	std::shared_ptr<StackedTexturePack2> parseStackedTexturePack(
		Game& game, const Value& elem)
	{
		auto texturePack = std::make_shared<StackedTexturePack2>();

		for (const auto& val : elem["texturePacks"sv])
		{
			auto texPack = game.Resources().getTexturePack(getStringVal(val));
			if (texPack != nullptr)
			{
				texturePack->addTexturePack(texPack);
			}
		}
		if (texturePack->size() == 0)
		{
			return nullptr;
		}
		return texturePack;
	}

	bool parseCompositeTexturePack(CompositeTexturePack2& compTexture,
		Game& game, const Value& elem)
	{
		if (elem.IsString() == false)
		{
			return false;
		}
		auto texturePack = game.Resources().getTexturePack(getStringViewVal(elem));
		if (texturePack == nullptr)
		{
			return false;
		}
		compTexture.addTexturePack(texturePack);
		return true;
	}

	void parseCompositeTexturePackGroup(CompositeTexturePack2& compTexture,
		Game& game, const Value& elem)
	{
		uint32_t TexturePackCount = 0;

		if (elem.HasMember("texturePacks"sv) == true)
		{
			const auto& texturePacksElem = elem["texturePacks"sv];
			if (isValidString(texturePacksElem) == true)
			{
				if (parseCompositeTexturePack(compTexture, game, texturePacksElem) == true)
				{
					TexturePackCount++;
				}
			}
			else if (texturePacksElem.IsArray() == true)
			{
				for (const auto& val : texturePacksElem)
				{
					if (parseCompositeTexturePack(compTexture, game, val) == true)
					{
						TexturePackCount++;
					}
				}
			}
		}

		if (TexturePackCount == 0)
		{
			return;
		}

#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
		if (isValidString(elem, "file") == true)
		{
			bool fixLayerOrdering = getBoolKey(elem, "fixLayerOrdering", true);
			compTexture.addGroup(elem["file"sv].GetStringView(), fixLayerOrdering);
			return;
		}
#endif
		auto groupIdx = compTexture.getCompositeTextureGroupSize();
		if (compTexture.addGroup(TexturePackCount) == true &&
			isValidArray(elem, "directionLayerOrders") == true)
		{
			std::vector<int8_t> layerOrder;
			size_t numLayers = compTexture.getLayerCount(groupIdx);
			size_t orderIdx = 0;
			for (const auto& layerOrderElem : elem["directionLayerOrders"sv])
			{
				if (layerOrderElem.IsArray() == true)
				{
					layerOrder.resize(layerOrder.size() + numLayers, -1);
					size_t subOrderIdx = orderIdx;
					size_t layerIdx = 0;
					for (const auto& val : layerOrderElem)
					{
						if (val.IsNumber() == true)
						{
							auto newIdx = (int8_t)std::clamp(getIntVal(val, -1), -1, 127);
							if ((int32_t)newIdx < (int32_t)numLayers)
							{
								layerOrder[subOrderIdx] = newIdx;
								subOrderIdx++;
								layerIdx++;
							}
							if (layerIdx >= numLayers)
							{
								break;
							}
						}
					}
					orderIdx += numLayers;
				}
			}
			compTexture.setLayersOrders(layerOrder);
		}
	}

	std::shared_ptr<CompositeTexturePack2> getCompositeTexturePackObj(
		Game& game, const Value& elem)
	{
		auto compTexture = std::make_shared<CompositeTexturePack2>();

		if (isValidArray(elem, "groups") == false)
		{
			parseCompositeTexturePackGroup(*compTexture, game, elem);
		}
		else
		{
			for (const auto& val : elem["groups"sv])
			{
				if (val.IsObject() == true)
				{
					parseCompositeTexturePackGroup(*compTexture, game, val);
				}
			}
		}
		if (compTexture->size() == 0)
		{
			return nullptr;
		}
		return compTexture;
	}

	std::shared_ptr<TexturePack> getTexturePackObj(Game& game, const Value& elem)
	{
		auto type = getTexturePackType(elem);
		switch (type)
		{
		case TexturePackType::Stacked:
		{
			return parseStackedTexturePack(game, elem);
		}
		case TexturePackType::Composite:
		{
			return getCompositeTexturePackObj(game, elem);
		}
		default:
			break;
		}

		std::unique_ptr<TexturePack> texturePack;

		if (type == TexturePackType::BitmapFont)
		{
			texturePack = parseBitmapFontTexturePack(game, elem);
		}
		else if (elem.HasMember("imageContainer"sv) == true)
		{
			texturePack = parseImageContainerTexturePack(game, elem);
		}
		else if (elem.HasMember("texture"sv) == true)
		{
			if (elem["texture"sv].IsArray() == true)
			{
				texturePack = parseMultiTextureTexturePack(game, elem);
			}
			else
			{
				texturePack = parseSingleTextureTexturePack(game, elem);
			}
		}
		if (texturePack == nullptr)
		{
			return nullptr;
		}
		if (isValidArray(elem, "rects") == true)
		{
			auto simpleTexturePack = dynamic_cast<SimpleTexturePack*>(texturePack.get());
			auto globalOffset = getVector2fKey<sf::Vector2f>(elem, "offset");
			bool invertOffsets = getBoolKey(elem, "invertOffsets");
			bool absoluteOffsets = getBoolKey(elem, "absoluteOffsets");
			auto texturePack2 = std::make_unique<RectTexturePack>(
				std::move(texturePack), absoluteOffsets);
			for (const auto& val : elem["rects"sv])
			{
				if (val.IsArray() == true)
				{
					auto rect = getIntRectVal(val);
					if (rect.width > 0 && rect.height > 0)
					{
						texturePack2->addRect(0, rect, {});
					}
				}
				else if (val.IsObject() == true)
				{
					auto rect = getIntRectKey(val, "rect");
					if (rect.width > 0 && rect.height > 0)
					{
						auto index = getUIntKey(val, "index");
						auto offset = globalOffset + getVector2fKey<sf::Vector2f>(val, "offset");
						if (invertOffsets == true)
						{
							offset.x = -offset.x;
							offset.y = -offset.y;
						}
						texturePack2->addRect(index, rect, offset);
					}
				}
			}
			if (simpleTexturePack != nullptr)
			{
				simpleTexturePack->setSize(texturePack2->size());
			}
			if (isValidArray(elem, "groups") == true)
			{
				auto animInfo = ((TexturePack*)texturePack2.get())->getAnimation(-1, -1);
				for (const auto& val : elem["groups"sv])
				{
					animInfo.indexRange = getFramesKey(val, "range", animInfo.indexRange);
					animInfo.animType = getAnimationTypeKey(val, "animationType", animInfo.animType);
					auto directions = getUIntKey(val, "directions");
					texturePack2->addGroup(
						animInfo.indexRange.first, animInfo.indexRange.second,
						directions, animInfo.animType
					);
				}
			}
			texturePack = std::move(texturePack2);
		}
		if (elem.HasMember("textureIndexes"sv) == true ||
			elem.HasMember("utf8Indexes"sv) == true ||
			elem.HasMember("utf8IndexFile"sv) == true)
		{
			auto texturePack2 = std::make_unique<IndexedTexturePack2>(
				std::move(texturePack),
				getBoolKey(elem, "onlyUseIndexed", true),
				getBoolKey(elem, "translateAnimatedIndexes", true)
			);

			std::string str;
			std::string_view strIndexes;
			if (isValidString(elem, "utf8Indexes") == true)
			{
				strIndexes = getStringViewVal(elem["utf8Indexes"sv]);
			}
			else if (isValidString(elem, "utf8IndexFile") == true)
			{
				str = FileUtils::readText(elem["utf8IndexFile"sv].GetStringView());
				strIndexes = str;
			}
			for (auto it = strIndexes.begin(), itEnd = strIndexes.end(); it < itEnd;)
			{
				sf::Uint32 ch;
				it = sf::Utf8::decode(it, itEnd, ch);
				texturePack2->mapTextureIndex(ch);
			}
			if (isValidArray(elem, "textureIndexes") == true)
			{
				for (const auto& val : elem["textureIndexes"sv])
				{
					if (val.IsUint() == true)
					{
						texturePack2->mapTextureIndex(val.GetUint());
					}
					else if (val.IsArray() == true &&
						val.Size() == 2 &&
						val[0].IsUint() &&
						val[1].IsUint())
					{
						texturePack2->mapTextureIndex(val[0].GetUint(), val[1].GetUint());
					}
				}
			}
			texturePack = std::move(texturePack2);
		}
		if (isValidArray(elem, "animatedTextures") == true)
		{
			auto animTexturePack = dynamic_cast<IndexedTexturePack2*>(texturePack.get());
			if (animTexturePack == nullptr)
			{
				auto texturePack2 = std::make_unique<IndexedTexturePack2>(
					std::move(texturePack),
					getBoolKey(elem, "onlyUseIndexed"),
					getBoolKey(elem, "translateAnimatedIndexes")
				);
				animTexturePack = texturePack2.get();
				texturePack = std::move(texturePack2);
			}
			for (const auto& animVal : elem["animatedTextures"sv])
			{
				if (isValidArray(animVal, "indexes") == false)
				{
					continue;
				}
				std::vector<uint32_t> indexes;
				for (const auto& indexVal : animVal["indexes"sv])
				{
					indexes.push_back(getUIntVal(indexVal));
				}
				auto animIndex = getUIntKey(animVal, "index", indexes.front());
				auto refresh = getTimeKey(animVal, "refresh", sf::milliseconds(50));
				animTexturePack->addAnimatedTexture(animIndex, refresh, indexes);
			}
		}
		return texturePack;
	}

	void parseTexturePack(Game& game, const Value& elem)
	{
		Parser::parseTexturePack(game, elem, getTexturePackObj);
	}
}
