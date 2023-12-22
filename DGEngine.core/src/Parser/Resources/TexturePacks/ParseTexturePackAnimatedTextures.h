#pragma once

#include "Json/JsonParser.h"
#include <memory>
#include "Parser/Utils/ParseUtils.h"
#include "Resources/TexturePacks/AnimatedTexturePack.h"
#include <type_traits>

namespace Parser
{
	template<class AnimatedTP = AnimatedTexturePack>
	void parseTexturePackAnimatedTextures(std::unique_ptr<TexturePack>& texturePack, const rapidjson::Value& elem)
	{
		static_assert(std::is_base_of_v<AnimatedTexturePack, AnimatedTP>);

		using namespace std::literals;

		if (isValidArray(elem, "animatedTextures") == false)
		{
			return;
		}

		auto texturePack2 = dynamic_cast<AnimatedTP*>(texturePack.get());
		if (texturePack2 == nullptr)
		{
			texturePack = std::make_unique<AnimatedTP>(std::move(texturePack));
			texturePack2 = dynamic_cast<AnimatedTP*>(texturePack.get());
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
			auto refresh = getTimeUKey(animVal, "refresh", sf::milliseconds(50));
			texturePack2->addAnimatedTexture(animIndex, refresh, indexes);
		}
	}
}
