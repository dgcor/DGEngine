#include "ParseTexturePack.h"
#include "FileUtils.h"
#include "Game/LevelHelper.h"
#include "ImageContainers/CelImageContainer.h"
#include "Min.h"
#include "ParseImageContainer.h"
#include "ParseTexture.h"
#include "TexturePacks/BitmapFontTexturePack.h"
#include "TexturePacks/CachedTexturePack.h"
#include "TexturePacks/IndexedTexturePack.h"
#include "TexturePacks/RectTexturePack.h"
#include "TexturePacks/SimpleTexturePack.h"
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
				std::string fromId(elem["fromId"].GetString());
				std::string id(elem["id"].GetString());
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
		if (elem.HasMember("palette") == true)
		{
			pal = game.Resources().getPalette(elem["palette"].GetString());
		}

		bool useIndexedImages = pal != nullptr && game.Shaders().hasSpriteShader();

		if (imgVec.size() == 1)
		{
			return std::make_unique<CachedTexturePack>(imgVec.front(), offset, pal, useIndexedImages);
		}
		else
		{
			return std::make_unique<CachedMultiTexturePack>(imgVec, offset, pal, useIndexedImages);
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
			palette = game.Resources().getPalette(elem["palette"].GetString());
		}
		auto isIndexed = palette != nullptr && getBoolKey(elem, "indexed");

		auto frames = getFramesKey(elem, "frames");
		if (frames.first == 0 || frames.second == 0)
		{
			frames.first = frames.second = 1;
		}
		auto offset = getVector2fKey<sf::Vector2f>(elem, "offset");
		auto startIndex = getUIntKey(elem, "startIndex");
		bool isHorizontal = getStringKey(elem, "direction") == "horizontal";

		auto texturePack = std::make_unique<SimpleTexturePack>(
			texture, frames, offset, startIndex, isHorizontal, palette, isIndexed);

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
				bool isHorizontal = getStringKey(val, "direction") == "horizontal";
				texturePack->addTexturePack(
					texture, frames, offset, startIndex, isHorizontal);
			}
		}
		if (texturePack->getTextureCount() == 0)
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
			palette = game.Resources().getPalette(elem["palette"].GetString());
		}

		auto isIndexed = palette != nullptr && getBoolKey(elem, "indexed");

		auto texturePack = std::make_unique<SimpleMultiTexturePack>(palette, isIndexed);
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
			if (isValidId(textureId) == true)
			{
				game.Resources().addTexture(textureId, texture, getStringViewKey(elem, "resource"));
			}
		}

		auto rows = getIntKey(elem, "rows", 16);
		auto cols = getIntKey(elem, "cols", 16);
		bool isVertical = getStringKey(elem, "direction") == "vertical";

		std::shared_ptr<BitmapFontTexturePack> texturePack;

		if (elem.HasMember("charSizeFile") == true)
		{
			NumberVector<uint8_t> charSizes(getStringViewVal(elem["charSizeFile"]), 0, 258);
			texturePack = std::make_shared<BitmapFontTexturePack>(
				texture, rows, cols, isVertical, charSizes.getContainer());
		}
		else
		{
			if (img.getSize().x == 0)
			{
				img = texture->copyToImage();
			}
			texturePack = std::make_shared<BitmapFontTexturePack>(
				texture, rows, cols, isVertical, img);
		}
		return texturePack;
	}

	std::shared_ptr<TexturePack> parseTexturePackObj(Game& game, const Value& elem)
	{
		if (getBoolKey(elem, "font") == true)
		{
			return parseBitmapFontTexturePackObj(game, elem);
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
			auto texturePack2 = std::make_unique<RectTexturePack>(std::move(texturePack));
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
			texturePack = std::move(texturePack2);
		}
		if (isValidArray(elem, "textureIndexes") == true)
		{
			auto texturePack2 = std::make_unique<IndexedTexturePack>(
				std::move(texturePack), getBoolKey(elem, "onlyUseIndexed", true));
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
			id = elem["id"].GetString();
		}
		else
		{
			if (isValidString(elem, "file") == false)
			{
				return;
			}
			std::string file(elem["file"].GetString());
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

	void parseLevelLayer(Game& game, const Value& elem,
		std::vector<std::shared_ptr<TexturePack>>& texturePackLayers)
	{
		size_t index = getUIntKey(elem, "index");
		if (index < texturePackLayers.size())
		{
			texturePackLayers[index] = game.Resources().getTexturePack(
				getStringKey(elem, "texturePack"));
		}
	}

	bool getOrParseLevelTexturePack(Game& game, const Value& elem,
		const char* idKeyLayers, std::vector<std::shared_ptr<TexturePack>>& texturePackLayers,
		std::pair<int32_t, int32_t>& tileSize)
	{
		texturePackLayers.resize(Level::NumberOfLayers - 1);
		bool success = false;

		if (isValidString(elem, "min") == true)
		{
			// l4.min and town.min contain 16 blocks, all others 10.
			auto minBlocks = getUIntKey(elem, "minBlocks", 10);
			if (minBlocks != 10 && minBlocks != 16)
			{
				minBlocks = 10;
			}
			Min min(getStringViewVal(elem["min"]), minBlocks);
			if (min.size() == 0)
			{
				return false;
			}

			auto pal = game.Resources().getPalette(elem["palette"].GetString());
			if (pal == nullptr)
			{
				return false;
			}

			std::shared_ptr<ImageContainer> imgCont;
			getOrParseImageContainer(game, elem, "imageContainer", imgCont);
			if (imgCont == nullptr)
			{
				return false;
			}

			bool useIndexedImages = game.Shaders().hasSpriteShader();
			CachedImagePack imgPack(imgCont.get(), pal, useIndexedImages);

			texturePackLayers[0] = LevelHelper::loadTilesetSprite(imgPack, min, false, false, true);
			texturePackLayers[1] = LevelHelper::loadTilesetSprite(imgPack, min, true, true, true);

			tileSize.first = 64;
			tileSize.second = 32;
			success = true;
		}
		else
		{
			tileSize = getVector2uKey<std::pair<int32_t, int32_t>>(
				elem, "tileSize", std::make_pair(64, 32));
		}

		if (elem.HasMember(idKeyLayers) == true)
		{
			const auto& layersElem = elem[idKeyLayers];
			if (layersElem.IsArray() == true)
			{
				for (const auto& val : layersElem)
				{
					parseLevelLayer(game, val, texturePackLayers);
				}
			}
			else if (layersElem.IsObject() == true)
			{
				parseLevelLayer(game, layersElem, texturePackLayers);
			}
			success = true;
		}
		return success;
	}
}
