#pragma once

#include "Json/JsonParser.h"
#include <memory>
#include "Parser/Utils/ParseUtils.h"
#include "Resources/TexturePacks/IndexedTexturePack.h"

namespace Parser
{
	template<class IndexedTP = IndexedTexturePack>
	void parseTexturePackAnimatedTextures(std::unique_ptr<TexturePack>& texturePack, const rapidjson::Value& elem)
	{
		using namespace std::literals;

		if (isValidArray(elem, "animatedTextures") == true)
		{
			auto animTexturePack = dynamic_cast<IndexedTP*>(texturePack.get());
			if (animTexturePack == nullptr)
			{
				auto texturePack2 = std::make_unique<IndexedTP>(
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
	}
}
