#pragma once

#include "Game/AnimationInfo.h"
#include "Json/JsonParser.h"
#include <memory>
#include "Parser/Utils/ParseUtils.h"
#include "Resources/TexturePacks/RectTexturePack.h"
#include <type_traits>

namespace Parser
{
	template<class RectTP = RectTexturePack>
	void parseTexturePackRects(std::unique_ptr<TexturePack>& texturePack, const rapidjson::Value& elem)
	{
		static_assert(std::is_base_of_v<RectTexturePack, RectTP>);

		using namespace std::literals;

		if (isValidArray(elem, "rects") == false)
		{
			return;
		}

		auto texturePack2 = dynamic_cast<RectTP*>(texturePack.get());
		if (texturePack2 == nullptr)
		{
			bool absoluteOffsets = getBoolKey(elem, "absoluteOffsets");

			texturePack = std::make_unique<RectTP>(
				std::move(texturePack), absoluteOffsets
			);
			texturePack2 = dynamic_cast<RectTP*>(texturePack.get());
		}

		auto globalOffset = getVector2fKey<sf::Vector2f>(elem, "offset");
		bool invertOffsets = getBoolKey(elem, "invertOffsets");

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
	}
}
