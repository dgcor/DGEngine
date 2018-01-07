#include "ParseTexturePack.h"
#include "Game/LevelHelper.h"
#include "ImageContainers/CelImageContainer.h"
#include "Min.h"
#include "ParseImageContainer.h"
#include "ParseTexture.h"
#include "TexturePacks/CachedTexturePack.h"
#include "TexturePacks/RectTexturePack.h"
#include "TexturePacks/SimpleIndexedTexturePack.h"
#include "TexturePacks/SimpleTexturePack.h"
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
						game.Resources().addTexturePack(id, obj);
					}
				}
			}
			return true;
		}
		return false;
	}

	std::shared_ptr<TexturePack> parseImageContainerTexturePack(
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

		std::shared_ptr<Palette> pal;
		if (elem.HasMember("palette") == true)
		{
			pal = game.Resources().getPalette(elem["palette"].GetString());
		}

		bool useIndexedImages = pal != nullptr && Shaders::supportsPalettes();

		if (imgVec.size() == 1)
		{
			return std::make_shared<CachedTexturePack>(imgVec.front(), pal, useIndexedImages);
		}
		else
		{
			return std::make_shared<CachedMultiTexturePack>(imgVec, pal, useIndexedImages);
		}
	}

	std::shared_ptr<TexturePack> parseSingleTextureTexturePack(
		Game& game, const Value& elem)
	{
		std::shared_ptr<sf::Texture> texture;
		getOrParseTexture(game, elem, "texture", texture);
		if (texture == nullptr)
		{
			return nullptr;
		}
		std::shared_ptr<TexturePack> texturePack;

		if (elem.HasMember("frames") == true)
		{
			auto frames = getFramesKey(elem, "frames");
			if (frames.first == 0 || frames.second == 0)
			{
				frames.first = frames.second = 1;
			}
			bool isHorizontal = getStringKey(elem, "direction") == "horizontal";

			texturePack = std::make_shared<SimpleTexturePack>(
				texture, frames.first, frames.second, isHorizontal);
		}
		else if (elem.HasMember("rects") == true)
		{
			auto texturePack2 = std::make_shared<RectTexturePack>(texture);
			texturePack = texturePack2;

			const auto& elemRects = elem["rects"];

			if (elemRects.IsArray() == true)
			{
				for (const auto& val : elemRects)
				{
					auto rect = getIntRectVal(val);
					if (rect.width > 0 && rect.height > 0)
					{
						texturePack2->add(rect);
					}
				}
			}
		}
		if (texturePack->totalSize() == 0)
		{
			return nullptr;
		}
		return texturePack;
	}

	bool parseMultiTextureTexturePackHelper(Game& game, const Value& elem,
		SimpleMultiTexturePack* texturePack)
	{
		for (const auto& val : elem["texture"])
		{
			std::shared_ptr<sf::Texture> texture;
			getOrParseTexture(game, val, "texture", texture);
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
				bool isHorizontal = getStringKey(val, "direction") == "horizontal";
				texturePack->addTexturePack(
					texture, frames.first, frames.second, isHorizontal);
			}
		}
		if (texturePack->getTextureCount() == 0)
		{
			return false;
		}
		return true;
	}

	std::shared_ptr<TexturePack> parseMultiTextureTexturePack(
		Game& game, const Value& elem)
	{
		std::shared_ptr<Palette> palette;
		if (isValidString(elem, "palette") == true)
		{
			palette = game.Resources().getPalette(elem["palette"].GetString());
		}

		auto isIndexed = palette != nullptr && getBoolKey(elem, "indexed");

		if (isValidArray(elem, "textureIndexes") == true)
		{
			auto texturePack = std::make_shared<SimpleIndexedMultiTexturePack>(palette, isIndexed);
			if (parseMultiTextureTexturePackHelper(game, elem, texturePack.get()) == false)
			{
				return nullptr;
			}
			for (const auto& val : elem["textureIndexes"])
			{
				if (val.IsUint() == true)
				{
					texturePack->addTextureIndex(val.GetUint());
				}
			}
			return texturePack;
		}
		else
		{
			auto texturePack = std::make_shared<SimpleMultiTexturePack>(palette, isIndexed);
			if (parseMultiTextureTexturePackHelper(game, elem, texturePack.get()) == false)
			{
				return nullptr;
			}
			return texturePack;
		}
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

		std::shared_ptr<TexturePack> texturePack;

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
			else
			{
				texturePack = parseSingleTextureTexturePack(game, elem);
			}
		}
		if (texturePack == nullptr)
		{
			return;
		}
		game.Resources().addTexturePack(id, texturePack);
	}

	void getOrParseLevelTexturePack(Game& game, const Value& elem,
		const char* idKeyBottom, const char* idKeyTop,
		std::shared_ptr<TexturePack>& texturePackBottom,
		std::shared_ptr<TexturePack>& texturePackTop)
	{
		if (isValidString(elem, idKeyBottom) == true &&
			isValidString(elem, idKeyTop) == true)
		{
			texturePackBottom = game.Resources().getTexturePack(elem[idKeyBottom].GetString());
			texturePackTop = game.Resources().getTexturePack(elem[idKeyTop].GetString());
		}
		else if (isValidString(elem, "min") == true)
		{
			// l4.min and town.min contain 16 blocks, all others 10.
			Min min(elem["min"].GetString(), getIntKey(elem, "minBlocks", 10));
			if (min.size() == 0)
			{
				return;
			}

			auto pal = game.Resources().getPalette(elem["palette"].GetString());
			if (pal == nullptr)
			{
				return;
			}

			std::shared_ptr<ImageContainer> imgCont;
			getOrParseImageContainer(game, elem, "imageContainer", imgCont);
			if (imgCont == nullptr)
			{
				return;
			}

			bool useIndexedImages = Shaders::supportsPalettes();
			CachedImagePack imgPack(imgCont.get(), pal, useIndexedImages);

			texturePackBottom = LevelHelper::loadTilesetSprite(imgPack, min, false, false, true);
			texturePackTop = LevelHelper::loadTilesetSprite(imgPack, min, true, true, true);
		}
	}
}
