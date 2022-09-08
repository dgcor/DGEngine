#pragma once

#include <cassert>
#include "ParseBitmapFontTexturePack.h"
#include "ParseCompositeTexturePack.h"
#include "ParseImageContainerTexturePack.h"
#include "ParseMultiTextureTexturePack.h"
#include "ParseSingleTextureTexturePack.h"
#include "ParseStackedTexturePack.h"
#include "ParseTexturePackAnimatedTextures.h"
#include "ParseTexturePackIndexes.h"
#include "ParseTexturePackRects.h"
#include "Parser/Utils/ParseUtils.h"
#include "Resources/TexturePacks/RectTexturePack.h"
#include "Resources/TexturePacks/SimpleTexturePack.h"

namespace Parser
{
	enum class TexturePackType
	{
		Generic,
		BitmapFont,
		Stacked,
		Composite
	};

	TexturePackType getTexturePackType(const rapidjson::Value& elem);

	typedef TexturePackType(*getTexturePackTypeFuncPtr)(const rapidjson::Value& elem);

	template<class TPType = TexturePackType,
		class BitmapFontTP = BitmapFontTexturePack,
		class CompositeTP = CompositeTexturePack,
		class ImageCTP = ImageContainerTexturePack,
		class IndexedTP = IndexedTexturePack,
		class MultiImageCTP = MultiImageContainerTexturePack,
		class MultiTP = MultiTexturePack,
		class RectTP = RectTexturePack,
		class SimpleTP = SimpleTexturePack,
		class SingleTP = SingleTexturePack,
		class StackedTP = StackedTexturePack>
	std::shared_ptr<TexturePack> getTexturePackObj(Game& game,
		const rapidjson::Value& elem, const getTexturePackTypeFuncPtr getTexturePackTypeFunc)
	{
		using namespace std::literals;

		assert(getTexturePackTypeFunc != nullptr);

		auto type = getTexturePackTypeFunc(elem);
		switch (type)
		{
		case TPType::Stacked:
		{
			return parseStackedTexturePack<StackedTP>(game, elem);
		}
		case TPType::Composite:
		{
			return getCompositeTexturePackObj<CompositeTP>(game, elem);
		}
		default:
			break;
		}

		std::unique_ptr<TexturePack> texturePack;

		if (type == TPType::BitmapFont)
		{
			texturePack = parseBitmapFontTexturePack<BitmapFontTP>(game, elem);
		}
		else if (elem.HasMember("imageContainer"sv) == true)
		{
			texturePack = parseImageContainerTexturePack<ImageCTP, MultiImageCTP>(game, elem);
		}
		else if (elem.HasMember("texture"sv) == true)
		{
			if (elem["texture"sv].IsArray() == true)
			{
				texturePack = parseMultiTextureTexturePack<MultiTP>(game, elem);
			}
			else
			{
				texturePack = parseSingleTextureTexturePack<SingleTP, SimpleTP>(game, elem);
			}
		}
		if (texturePack == nullptr)
		{
			return nullptr;
		}

		parseTexturePackRects<RectTP, SimpleTP>(texturePack, elem);
		parseTexturePackIndexes<IndexedTP>(texturePack, elem);
		parseTexturePackAnimatedTextures<IndexedTP>(texturePack, elem);

		return texturePack;
	}

	typedef std::shared_ptr<TexturePack>(*getTexturePackObjFuncPtr)(Game& game, const rapidjson::Value& elem);

	void parseTexturePackF(Game& game, const rapidjson::Value& elem, const getTexturePackObjFuncPtr getTexturePackObjFunc);
}
