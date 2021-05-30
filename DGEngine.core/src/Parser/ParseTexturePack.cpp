#include "ParseTexturePack.h"
#include <cassert>
#include "FileUtils.h"
#include "Game.h"
#include <SFML/System/Utf.hpp>
#include "TexturePacks/BitmapFontTexturePack.h"
#include "TexturePacks/CachedTexturePack.h"
#include "TexturePacks/CompositeTexturePack.h"
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
			return TexturePackType::Stacked;
		}
		return TexturePackType::Generic;
	}

	void parseTexturePackDirectionVector(const Value& elem, uint32_t& directions,
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
		auto offset = getVector2fKey<sf::Vector2f>(elem, "offset");

		if (imgVec.size() == 1)
		{
			auto texturePack = std::make_unique<CachedTexturePack>(
				imgVec.front(), offset, pal, useIndexedImages
			);
			return texturePack;
		}
		else
		{
			return std::make_unique<CachedMultiTexturePack>(
				imgVec, offset, pal, useIndexedImages
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
		parseTexturePackDirectionVector(elem, t.directions, t.directionsVec);
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

	bool parseCompositeTexturePack(CompositeTexturePack& compTexture,
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

	void parseCompositeTexturePackGroup(CompositeTexturePack& compTexture,
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

	std::shared_ptr<CompositeTexturePack> getCompositeTexturePackObj(
		Game& game, const Value& elem)
	{
		auto compTexture = std::make_shared<CompositeTexturePack>();

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

	void parseTexturePack(Game& game, const Value& elem,
		const getTexturePackObjFuncPtr getTexturePackObjFunc)
	{
		assert(getTexturePackObjFunc != nullptr);

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
		auto texturePack = getTexturePackObjFunc(game, elem);
		if (texturePack == nullptr)
		{
			return;
		}
		game.Resources().addTexturePack(id, std::move(texturePack), getStringViewKey(elem, "resource"));
	}

	void parseTexturePack(Game& game, const Value& elem)
	{
		parseTexturePack(game, elem, getTexturePackObj);
	}
}
