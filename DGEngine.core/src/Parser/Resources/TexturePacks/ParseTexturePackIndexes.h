#pragma once

#include "Game/Utils/FileUtils.h"
#include "Json/JsonParser.h"
#include <memory>
#include "Parser/Utils/ParseUtils.h"
#include "Resources/TexturePacks/IndexedTexturePack.h"

namespace Parser
{
	template<class IndexedTP = IndexedTexturePack>
	void parseTexturePackIndexes(std::unique_ptr<TexturePack>& texturePack, const rapidjson::Value& elem)
	{
		using namespace std::literals;

		if (elem.HasMember("textureIndexes"sv) == true ||
			elem.HasMember("utf8Indexes"sv) == true ||
			elem.HasMember("utf8IndexFile"sv) == true)
		{
			auto texturePack2 = std::make_unique<IndexedTP>(
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
	}
}
