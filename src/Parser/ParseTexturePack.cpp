#include "ParseTexturePack.h"
#include "FileUtils.h"
#include "Game.h"
#ifndef NO_DIABLO_FORMAT_SUPPORT
#include "CachedImagePack.h"
#include "Game/LevelHelper.h"
#include "ImageContainers/DT1ImageContainer.h"
#endif
#include <SFML/System/Utf.hpp>
#include "TexturePacks/BitmapFontTexturePack.h"
#include "TexturePacks/CachedTexturePack.h"
#include "TexturePacks/IndexedTexturePack.h"
#include "TexturePacks/RectTexturePack.h"
#include "TexturePacks/SimpleTexturePack.h"
#include "TexturePacks/StackedTexturePack.h"
#include "Utils/NumberVector.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseDirectionVector(const Value& elem, uint32_t& directions,
		std::vector<std::pair<uint32_t, uint32_t>>& directionsVec)
	{
		if (isValidArray(elem, "directions") == true)
		{
			for (const auto& dirVal : elem["directions"sv])
			{
				auto directionRange = getVector2uVal<std::pair<uint32_t, uint32_t>>(dirVal);
				if (directionRange.second < directionRange.first)
				{
					continue;
				}
				directionsVec.push_back(directionRange);
			}
		}
		else
		{
			directions = getUIntKey(elem, "directions");
		}
	}

	bool parseTexturePackFromId(Game& game, const Value& elem)
	{
		if (isValidString(elem, "fromId") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				auto fromId = elem["fromId"sv].GetStringView();
				auto id = elem["id"sv].GetStringView();
				if (fromId != id && isValidId(id) == true)
				{
					auto obj = game.Resources().getTexturePack(fromId);
					if (obj != nullptr)
					{
						game.Resources().addTexturePack(id, obj, getStringViewKey(elem, "resource"));
					}
				}
			}
			return true;
		}
		return false;
	}

#ifndef NO_DIABLO_FORMAT_SUPPORT
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

#ifndef NO_DIABLO_FORMAT_SUPPORT
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
			auto texturePack = std::make_unique<CachedTexturePack>(
				imgVec.front(), offset, pal, useIndexedImages, normalizeDirections
			);

#ifndef NO_DIABLO_FORMAT_SUPPORT
			auto dt1ImgCont = dynamic_cast<DT1ImageContainer*>(imgVec.front().get());
			if (dt1ImgCont == nullptr)
			{
				return texturePack;
			}

			auto texturePack2 = std::make_unique<IndexedTexturePack>(
				std::move(texturePack), true, false);

			parseDT1ImageContainerTexturePack(game, elem, *dt1ImgCont, *texturePack2);

			return texturePack2;
#else
			return texturePack;
#endif
		}
		else
		{
			return std::make_unique<CachedMultiTexturePack>(
				imgVec, offset, pal, useIndexedImages, normalizeDirections
			);
		}
	}

	std::unique_ptr<TexturePack> parseSingleTextureTexturePack(
		Game& game, const Value& elem)
	{
		if (isValidString(elem, "texture") == false)
		{
			return nullptr;
		}
		MultiTexture t;
		t.texture = game.Resources().getTexture(elem["texture"sv].GetStringView());
		if (t.texture == nullptr)
		{
			return nullptr;
		}
		std::shared_ptr<Palette> palette;
		if (isValidString(elem, "palette") == true)
		{
			palette = game.Resources().getPalette(elem["palette"sv].GetStringView());
		}

		auto frames = getFramesKey(elem, "frames");
		if (frames.first == 0 || frames.second == 0)
		{
			frames.first = frames.second = 1;
		}
		t.offset = getVector2fKey<sf::Vector2f>(elem, "offset");
		t.startIndex = getUIntKey(elem, "startIndex");
		parseDirectionVector(elem, t.directions, t.directionsVec);
		t.horizontalDirection = getStringViewKey(elem, "direction") == "horizontal";
		t.animType = getAnimationTypeKey(elem, "animationType");
		t.refresh = getTimeKey(elem, "refresh");

		std::unique_ptr<TexturePack> texturePack;

		if (frames.first == 1 &&
			frames.second == 1 &&
			t.startIndex == 0)
		{
			texturePack = std::make_unique<SimpleTexturePack>(std::move(t), palette);
		}
		else
		{
			texturePack = std::make_unique<SingleTexturePack>(std::move(t), frames, palette);
		}

		if (texturePack->size() == 0)
		{
			return nullptr;
		}
		return texturePack;
	}

	std::unique_ptr<TexturePack> parseMultiTextureTexturePack(
		Game& game, const Value& elem)
	{
		std::shared_ptr<Palette> palette;
		if (isValidString(elem, "palette") == true)
		{
			palette = game.Resources().getPalette(elem["palette"sv].GetStringView());
		}

		auto texturePack = std::make_unique<MultiTexturePack>(palette);
		auto globalOffset = getVector2fKey<sf::Vector2f>(elem, "offset");

		for (const auto& val : elem["texture"sv])
		{
			if (isValidString(val, "id") == false)
			{
				continue;
			}
			MultiTexture t;
			t.texture = game.Resources().getTexture(val["id"sv].GetStringView());
			if (t.texture == nullptr)
			{
				continue;
			}
			if (val.HasMember("frames"sv) == true)
			{
				auto frames = getFramesKey(val, "frames");
				if (frames.first == 0 || frames.second == 0)
				{
					frames.first = frames.second = 1;
				}
				t.offset = globalOffset + getVector2fKey<sf::Vector2f>(val, "offset");
				t.startIndex = getUIntKey(val, "startIndex");
				t.directions = getUIntKey(elem, "directions");
				t.horizontalDirection = getStringViewKey(val, "direction") == "horizontal";
				t.animType = getAnimationTypeKey(val, "animationType");
				texturePack->addTexturePack(std::move(t), frames);
			}
		}
		if (texturePack->size() == 0)
		{
			return nullptr;
		}
		return texturePack;
	}

	std::unique_ptr<TexturePack> parseBitmapFontTexturePack(
		Game& game, const Value& elem)
	{
		if (isValidString(elem, "texture") == false)
		{
			return nullptr;
		}
		auto texture = game.Resources().getTexture(elem["texture"sv].GetStringView());
		if (texture == nullptr)
		{
			return nullptr;
		}

		std::shared_ptr<Palette> palette;
		if (isValidString(elem, "palette") == true)
		{
			palette = game.Resources().getPalette(elem["palette"sv].GetStringView());
		}

		auto rows = getIntKey(elem, "rows", 16);
		auto cols = getIntKey(elem, "cols", 16);
		auto newLine = (int16_t)getIntKey(elem, "newLine", 0);
		auto space = (int16_t)getIntKey(elem, "space", 0);
		auto tab = (int16_t)getIntKey(elem, "tab", 0);
		bool isVertical = getStringViewKey(elem, "direction") == std::string_view("vertical");

		std::unique_ptr<BitmapFontTexturePack> texturePack;

		if (elem.HasMember("charSizeFile"sv) == true)
		{
			NumberVector<uint8_t> charSizes(getStringViewVal(elem["charSizeFile"sv]), 0, 0x7FFF);
			auto startPos = getUIntKey(elem, "charSizeFileStart", 0);
			auto skipNBytes = getUIntKey(elem, "charSizeFileSkip", 1);
			texturePack = std::make_unique<BitmapFontTexturePack>(
				texture, palette, rows, cols, newLine, space, tab,
				isVertical, charSizes.getContainer(), startPos, skipNBytes
			);
		}
		else
		{
			texturePack = std::make_unique<BitmapFontTexturePack>(
				texture, palette, rows, cols, newLine, space, tab, isVertical);
		}
		return texturePack;
	}

	std::shared_ptr<StackedTexturePack> parseStackedTexturePack(
		Game& game, const Value& elem)
	{
		auto texturePack = std::make_shared<StackedTexturePack>();

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

	std::shared_ptr<TexturePack> getTexturePackObj(Game& game, const Value& elem)
	{
		if (isValidArray(elem, "texturePacks") == true)
		{
			return parseStackedTexturePack(game, elem);
		}

		std::unique_ptr<TexturePack> texturePack;

		if (getBoolKey(elem, "font") == true)
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
			auto texturePack2 = std::make_unique<IndexedTexturePack>(
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
				it = std::move(sf::Utf8::decode(it, itEnd, ch));
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
			auto animTexturePack = dynamic_cast<IndexedTexturePack*>(texturePack.get());
			if (animTexturePack == nullptr)
			{
				auto texturePack2 = std::make_unique<IndexedTexturePack>(
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
		if (parseTexturePackFromId(game, elem) == true)
		{
			return;
		}
		std::string id;
		if (isValidString(elem, "id") == true)
		{
			id = elem["id"sv].GetStringView();
		}
		else
		{
			if (isValidString(elem, "file") == false)
			{
				return;
			}
			auto file = elem["file"sv].GetStringView();
			if (getIdFromFile(file, id) == false)
			{
				return;
			}
		}
		if (isValidId(id) == false)
		{
			return;
		}
		if (game.Resources().hasTexturePack(id) == true)
		{
			return;
		}
		auto texturePack = getTexturePackObj(game, elem);
		if (texturePack == nullptr)
		{
			return;
		}
		game.Resources().addTexturePack(id, std::move(texturePack), getStringViewKey(elem, "resource"));
	}
}
