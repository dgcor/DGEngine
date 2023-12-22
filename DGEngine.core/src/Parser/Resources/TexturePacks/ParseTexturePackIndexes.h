#pragma once

#include "Game/Utils/FileUtils.h"
#include "Json/JsonParser.h"
#include <memory>
#include "Parser/Utils/ParseUtils.h"
#include "Resources/TexturePacks/IndexedTexturePack.h"
#include <type_traits>

namespace Parser
{
	template<class IndexedTP = IndexedTexturePack>
	void parseTexturePackIndexes(std::unique_ptr<TexturePack>& texturePack, const rapidjson::Value& elem)
	{
		static_assert(std::is_base_of_v<IndexedTexturePack, IndexedTP>);

		using namespace std::literals;

		if (elem.HasMember("textureIndexes"sv) == false &&
			elem.HasMember("textureIndexRange"sv) == false &&
			elem.HasMember("utf8Indexes"sv) == false &&
			elem.HasMember("utf8IndexFile"sv) == false)
		{
			return;
		}

		auto texturePack2 = dynamic_cast<IndexedTP*>(texturePack.get());
		if (texturePack2 == nullptr)
		{
			texturePack = std::make_unique<IndexedTP>(
				std::move(texturePack),
				getBoolKey(elem, "onlyUseIndexed", true)
			);
			texturePack2 = dynamic_cast<IndexedTP*>(texturePack.get());
		}

		auto reverseMapIndex = getBoolKey(elem, "reverseMapIndex");

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
			texturePack2->mapTextureIndex(ch, reverseMapIndex);
		}
		if (isValidArray(elem, "textureIndexes") == true)
		{
			for (const auto& val : elem["textureIndexes"sv])
			{
				if (val.IsUint() == true)
				{
					texturePack2->mapTextureIndex(val.GetUint(), reverseMapIndex);
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
		else if (isValidArray(elem, "textureIndexRange") == true)
		{
			auto indexRangeStart = getIntKey(elem, "textureIndexRangeStart", -1);
			auto indexRange = getVector2uVal<PairInt32>(elem["textureIndexRange"sv]);

			texturePack2->mapTextureIndexRange(indexRange, indexRangeStart, reverseMapIndex);
		}
	}
}
