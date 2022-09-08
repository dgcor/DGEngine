#pragma once

#include "Game/AnimationInfo.h"
#include "Json/JsonParser.h"
#include <memory>
#include "Parser/Utils/ParseUtils.h"
#include "Resources/TexturePacks/RectTexturePack.h"
#include "Resources/TexturePacks/SimpleTexturePack.h"

namespace Parser
{
	template<class RectTP = RectTexturePack, class SimpleTP = SimpleTexturePack>
	void parseTexturePackRects(std::unique_ptr<TexturePack>& texturePack, const rapidjson::Value& elem)
	{
		using namespace std::literals;

		if (isValidArray(elem, "rects") == true)
		{
			auto simpleTexturePack = dynamic_cast<SimpleTP*>(texturePack.get());

			auto globalOffset = getVector2fKey<sf::Vector2f>(elem, "offset");
			bool invertOffsets = getBoolKey(elem, "invertOffsets");
			bool absoluteOffsets = getBoolKey(elem, "absoluteOffsets");

			auto texturePack2 = std::make_unique<RectTP>(
				std::move(texturePack), absoluteOffsets
			);

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
	}
}
