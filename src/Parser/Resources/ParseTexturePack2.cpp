#include "ParseTexturePack2.h"
#include "Parser/Resources/ParseResource.h"
#include "Parser/Resources/ParseTexturePack.h"
#include "Parser/Resources/TexturePacks/ParseBitmapFontTexturePack.h"
#include "Parser/Resources/TexturePacks/ParseCompositeTexturePack2.h"
#include "Parser/Resources/TexturePacks/ParseImageContainerTexturePack2.h"
#include "Parser/Resources/TexturePacks/ParseMultiTextureTexturePack.h"
#include "Parser/Resources/TexturePacks/ParseSingleTextureTexturePack.h"
#include "Parser/Resources/TexturePacks/ParseStackedTexturePack.h"
#include "Parser/Resources/TexturePacks/ParseTexturePackAnimatedTextures.h"
#include "Parser/Resources/TexturePacks/ParseTexturePackGroups.h"
#include "Parser/Resources/TexturePacks/ParseTexturePackIndexes.h"
#include "Parser/Resources/TexturePacks/ParseTexturePackRects.h"
#include "Parser/Resources/TexturePacks/ParseTexturePacks.h"
#include "Parser/Utils/ParseUtils.h"
#include "Resources/TexturePacks/CompositeTexturePack2.h"
#include "Resources/TexturePacks/IndexedTexturePack2.h"
#include "Resources/TexturePacks/StackedTexturePack2.h"

namespace Parser2
{
	using namespace Parser;
	using namespace rapidjson;
	using namespace std::literals;

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

	std::shared_ptr<TexturePack> getTexturePackObj(Game& game, const Value& elem)
	{
		auto type = getTexturePackType(elem);
		switch (type)
		{
		case TexturePackType::Stacked:
		{
			return parseStackedTexturePack<StackedTexturePack2>(game, elem);
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
					parseTexturePackAnimatedTextures(texturePack, elem);
				}
				break;
			}
			case str2int16("groups"):
			{
				if (parsedGroups == false)
				{
					parsedGroups = true;
					parseTexturePackGroups(texturePack, elem);
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
					parseTexturePackIndexes<IndexedTexturePack2>(texturePack, elem);
				}
				break;
			}
			case str2int16("rects"):
			{
				if (parsedRects == false)
				{
					parsedRects = true;
					parseTexturePackRects(texturePack, elem);
				}
				break;
			}
			default:
				break;
			}
		}

		return texturePack;
	}

	void parseTexturePack(Game& game, const Value& elem)
	{
		if (parseResourceFromId<std::shared_ptr<TexturePack>>(game.Resources(), elem) == true)
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
