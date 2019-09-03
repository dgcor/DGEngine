#include "ParseTexturePack.h"
#include "Game.h"
#ifndef NO_DIABLO_FORMAT_SUPPORT
#include "ImageContainers/DT1ImageContainer.h"
#endif
#include "ParseImageContainer.h"
#include "ParseTexture.h"
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

	bool parseTexturePackFromId(Game& game, const Value& elem)
	{
		if (isValidString(elem, "fromId") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				auto fromId = elem["fromId"].GetStringStr();
				auto id = elem["id"].GetStringStr();
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
#endif

	std::unique_ptr<TexturePack> parseImageContainerTexturePack(
		Game& game, const Value& elem)
	{
		std::vector<std::shared_ptr<ImageContainer>> imgVec;

		const auto& imgElem = elem["imageContainer"];
		if (imgElem.IsString() == true)
		{
			std::shared_ptr<ImageContainer> imgCont;
			getOrParseImageContainer(game, elem, "imageContainer", imgCont);
			if (imgCont != nullptr)
			{
				imgVec.push_back(imgCont);
			}
		}
		else if (imgElem.IsArray() == true)
		{
			for (const auto& val : imgElem)
			{
				auto imgCont = game.Resources().getImageContainer(getStringVal(val));
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

		auto offset = getVector2fKey<sf::Vector2f>(elem, "offset");

		std::shared_ptr<Palette> pal;
		if (isValidString(elem, "palette") == true)
		{
			pal = game.Resources().getPalette(elem["palette"].GetStringStr());
		}

		bool useIndexedImages = pal != nullptr && game.Shaders().hasSpriteShader();
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
		Game& game, const Value& elem, const char* textureIdKey)
	{
		std::shared_ptr<sf::Texture> texture;
		getOrParseTexture(game, elem, textureIdKey, texture);
		if (texture == nullptr)
		{
			return nullptr;
		}
		std::shared_ptr<Palette> palette;
		if (isValidString(elem, "palette") == true)
		{
			palette = game.Resources().getPalette(elem["palette"].GetStringStr());
		}

		auto frames = getFramesKey(elem, "frames");
		if (frames.first == 0 || frames.second == 0)
		{
			frames.first = frames.second = 1;
		}
		auto offset = getVector2fKey<sf::Vector2f>(elem, "offset");
		auto startIndex = getUIntKey(elem, "startIndex");
		auto directions = getUIntKey(elem, "directions");
		bool isHorizontal = getStringViewKey(elem, "direction") == "horizontal";
		auto animType = getAnimationTypeKey(elem, "animationType");

		auto texturePack = std::make_unique<SimpleTexturePack>(
			texture, frames, offset, startIndex,
			directions, isHorizontal, animType, palette
		);

		if (texturePack->size() == 0)
		{
			return nullptr;
		}
		return texturePack;
	}

	bool parseMultiTextureTexturePackHelper(Game& game, const Value& elem,
		SimpleMultiTexturePack* texturePack)
	{
		auto globalOffset = getVector2fKey<sf::Vector2f>(elem, "offset");
		ImageCache cache;
		for (const auto& val : elem["texture"])
		{
			std::shared_ptr<sf::Texture> texture;
			getOrParseTexture(game, val, "id", texture, &cache);
			if (texture == nullptr)
			{
				return false;
			}
			if (val.HasMember("frames") == true)
			{
				auto frames = getFramesKey(val, "frames");
				if (frames.first == 0 || frames.second == 0)
				{
					frames.first = frames.second = 1;
				}
				auto offset = globalOffset + getVector2fKey<sf::Vector2f>(val, "offset");
				auto startIndex = getUIntKey(val, "startIndex");
				auto directions = getUIntKey(elem, "directions");
				bool isHorizontal = getStringViewKey(val, "direction") == "horizontal";
				auto animType = getAnimationTypeKey(val, "animationType");
				texturePack->addTexturePack(
					texture, frames, offset, startIndex,
					directions, isHorizontal, animType
				);
			}
		}
		if (texturePack->size() == 0)
		{
			return false;
		}
		return true;
	}

	std::unique_ptr<TexturePack> parseMultiTextureTexturePack(
		Game& game, const Value& elem)
	{
		std::shared_ptr<Palette> palette;
		if (isValidString(elem, "palette") == true)
		{
			palette = game.Resources().getPalette(elem["palette"].GetStringStr());
		}

		auto texturePack = std::make_unique<SimpleMultiTexturePack>(palette);
		if (parseMultiTextureTexturePackHelper(game, elem, texturePack.get()) == false)
		{
			return nullptr;
		}
		return texturePack;
	}

	std::shared_ptr<BitmapFontTexturePack> parseBitmapFontTexturePackObj(
		Game& game, const Value& elem)
	{
		sf::Image img;
		bool existingTexture = true;
		auto textureId = getStringKey(elem, "texture");
		auto texture = game.Resources().getTexture(textureId);
		if (texture == nullptr)
		{
			img = parseTextureImg(game, elem);
			auto imgSize = img.getSize();
			if (imgSize.x == 0 || imgSize.y == 0)
			{
				return nullptr;
			}
			texture = parseTextureObj(game, elem, img);
			if (texture == nullptr)
			{
				return nullptr;
			}
			existingTexture = false;
			if (isValidId(textureId) == true)
			{
				game.Resources().addTexture(textureId, texture, getStringViewKey(elem, "resource"));
			}
		}

		std::shared_ptr<Palette> palette;
		if (existingTexture == true && isValidString(elem, "palette") == true)
		{
			palette = game.Resources().getPalette(elem["palette"].GetStringStr());
		}

		auto rows = getIntKey(elem, "rows", 16);
		auto cols = getIntKey(elem, "cols", 16);
		bool isVertical = getStringKey(elem, "direction") == "vertical";

		std::shared_ptr<BitmapFontTexturePack> texturePack;

		if (elem.HasMember("charSizeFile") == true)
		{
			NumberVector<uint8_t> charSizes(getStringViewVal(elem["charSizeFile"]), 0, 0x7FFF);
			auto startPos = getUIntKey(elem, "charSizeFileStart", 0);
			auto skipNBytes = getUIntKey(elem, "charSizeFileSkip", 1);
			auto spaceSize = (uint8_t)getUIntKey(elem, "charSpaceSize", 0);
			auto newLineSize = (uint8_t)getUIntKey(elem, "charNewLineSize", 0);
			texturePack = std::make_shared<BitmapFontTexturePack>(
				texture, palette, rows, cols, isVertical, charSizes.getContainer(),
				startPos, skipNBytes, spaceSize, newLineSize
			);
		}
		else
		{
			if (img.getSize().x == 0)
			{
				img = texture->copyToImage();
			}
			texturePack = std::make_shared<BitmapFontTexturePack>(
				texture, palette, rows, cols, isVertical, img);
		}
		return texturePack;
	}

	std::shared_ptr<StackedTexturePack> parseStackedTexturePackObj(
		Game& game, const Value& elem)
	{
		auto texturePack = std::make_shared<StackedTexturePack>();

		for (const auto& val : elem["texturePacks"])
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

	std::shared_ptr<TexturePack> parseTexturePackObj(Game& game, const Value& elem)
	{
		if (getBoolKey(elem, "font") == true)
		{
			return parseBitmapFontTexturePackObj(game, elem);
		}
		else if (isValidArray(elem, "texturePacks") == true)
		{
			return parseStackedTexturePackObj(game, elem);
		}

		std::unique_ptr<TexturePack> texturePack;

		if (elem.HasMember("imageContainer") == true)
		{
			texturePack = parseImageContainerTexturePack(game, elem);
		}
		else if (elem.HasMember("texture") == true)
		{
			if (elem["texture"].IsArray() == true)
			{
				texturePack = parseMultiTextureTexturePack(game, elem);
			}
			else if (elem["texture"].IsObject() == true)
			{
				texturePack = parseSingleTextureTexturePack(game, elem["texture"], "id");
			}
			else
			{
				texturePack = parseSingleTextureTexturePack(game, elem, "texture");
			}
		}
		if (texturePack == nullptr)
		{
			return nullptr;
		}
		if (isValidArray(elem, "rects") == true)
		{
			auto globalOffset = getVector2fKey<sf::Vector2f>(elem, "offset");
			bool invertOffsets = getBoolKey(elem, "invertOffsets");
			bool absoluteOffsets = getBoolKey(elem, "absoluteOffsets");
			auto texturePack2 = std::make_unique<RectTexturePack>(
				std::move(texturePack), absoluteOffsets);
			for (const auto& val : elem["rects"])
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
			if (isValidArray(elem, "groups") == true)
			{
				AnimationType animType;
				auto range = ((TexturePack*)texturePack2.get())->getRange(-1, -1, animType);
				for (const auto& val : elem["groups"])
				{
					range = getFramesKey(val, "range", range);
					animType = getAnimationTypeKey(val, "animationType", animType);
					auto directions = getUIntKey(val, "directions");
					texturePack2->addGroup(range.first, range.second, directions, animType);
				}
			}
			texturePack = std::move(texturePack2);
		}
		if (isValidArray(elem, "textureIndexes") == true)
		{
			auto texturePack2 = std::make_unique<IndexedTexturePack>(
				std::move(texturePack),
				getBoolKey(elem, "onlyUseIndexed", true),
				getBoolKey(elem, "translateAnimatedIndexes", true)
			);
			for (const auto& val : elem["textureIndexes"])
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
			for (const auto& animVal : elem["animatedTextures"])
			{
				if (isValidArray(animVal, "indexes") == false)
				{
					continue;
				}
				std::vector<uint32_t> indexes;
				for (const auto& indexVal : animVal["indexes"])
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
			id = elem["id"].GetStringStr();
		}
		else
		{
			if (isValidString(elem, "file") == false)
			{
				return;
			}
			auto file = elem["file"].GetStringView();
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
		auto texturePack = parseTexturePackObj(game, elem);
		if (texturePack == nullptr)
		{
			return;
		}
		game.Resources().addTexturePack(id, std::move(texturePack), getStringViewKey(elem, "resource"));
	}
}
