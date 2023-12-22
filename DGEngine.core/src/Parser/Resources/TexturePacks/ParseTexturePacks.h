#pragma once

#include <cassert>
#include "ParseBitmapFontTexturePack.h"
#include "ParseCompositeTexturePack.h"
#include "ParseImageContainerTexturePack.h"
#include "ParseMultiTextureTexturePack.h"
#include "ParseSingleTextureTexturePack.h"
#include "ParseStackedTexturePack.h"
#include "ParseTexturePackAnimatedTextures.h"
#include "ParseTexturePackGroups.h"
#include "ParseTexturePackIndexes.h"
#include "ParseTexturePackRects.h"
#include "Parser/Utils/ParseUtils.h"
#include <ranges>
#include "Resources/TexturePacks/RectTexturePack.h"
#include "Utils/StringHash.h"

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
		class AnimatedTP = AnimatedTexturePack,
		class BitmapFontTP = BitmapFontTexturePack,
		class CompositeTP = CompositeTexturePack,
		class GroupTP = GroupTexturePack,
		class ImageCTP = ImageContainerTexturePack,
		class IndexedTP = IndexedTexturePack,
		class MultiTP = MultiTexturePack,
		class RectTP = RectTexturePack,
		class StackedTP = StackedTexturePack>
	std::shared_ptr<TexturePack> getTexturePackObj(Game& game,
		const rapidjson::Value& elem, const getTexturePackTypeFuncPtr getTexturePackTypeFunc)
	{
		static_assert(std::is_base_of_v<AnimatedTexturePack, AnimatedTP>);
		static_assert(std::is_base_of_v<BitmapFontTexturePack, BitmapFontTP>);
		static_assert(std::is_base_of_v<CompositeTexturePack, CompositeTP>);
		static_assert(std::is_base_of_v<GroupTexturePack, GroupTP>);
		static_assert(std::is_base_of_v<ImageContainerTexturePack, ImageCTP>);
		static_assert(std::is_base_of_v<IndexedTexturePack, IndexedTP>);
		static_assert(std::is_base_of_v<MultiTexturePack, MultiTP>);
		static_assert(std::is_base_of_v<RectTexturePack, RectTP>);
		static_assert(std::is_base_of_v<StackedTexturePack, StackedTP>);

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
			texturePack = parseImageContainerTexturePack<ImageCTP>(game, elem);
		}
		else if (elem.HasMember("texture"sv) == true)
		{
			if (elem["texture"sv].IsArray() == true)
			{
				texturePack = parseMultiTextureTexturePack<MultiTP>(game, elem);
			}
			else
			{
				texturePack = parseSingleTextureTexturePack<MultiTP>(game, elem);
			}
		}
		if (texturePack == nullptr)
		{
			return nullptr;
		}

		bool parsedAnimated = false;
		bool parsedGroups = false;
		bool parsedIndexes = false;
		bool parsedRects = false;

		for (const auto& it : std::ranges::subrange(elem.MemberBegin(), elem.MemberEnd()))
		{
			switch (str2int16(it.name.GetStringView()))
			{
			case str2int16("animatedTextures"):
			{
				if (parsedAnimated == false)
				{
					parsedAnimated = true;
					parseTexturePackAnimatedTextures<AnimatedTP>(texturePack, elem);
				}
				break;
			}
			case str2int16("groups"):
			{
				if (parsedGroups == false)
				{
					parsedGroups = true;
					parseTexturePackGroups<GroupTP>(texturePack, elem);
				}
				break;
			}
			case str2int16("textureIndexes"):
			case str2int16("textureIndexRange"):
			case str2int16("utf8Indexes"):
			case str2int16("utf8IndexFile"):
			{
				if (parsedIndexes == false)
				{
					parsedIndexes = true;
					parseTexturePackIndexes<IndexedTP>(texturePack, elem);
				}
				break;
			}
			case str2int16("rects"):
			{
				if (parsedRects == false)
				{
					parsedRects = true;
					parseTexturePackRects<RectTP>(texturePack, elem);
				}
				break;
			}
			default:
				break;
			}
		}

		return texturePack;
	}

	typedef std::shared_ptr<TexturePack>(*getTexturePackObjFuncPtr)(Game& game, const rapidjson::Value& elem);

	void parseTexturePackF(Game& game, const rapidjson::Value& elem, const getTexturePackObjFuncPtr getTexturePackObjFunc);
}
