#pragma once

#include "Game/Game.h"
#include "Json/JsonParser.h"
#include <memory>
#include "Parser/Utils/ParseUtils.h"
#include "Resources/TexturePacks/MultiTexturePack.h"

namespace Parser
{
	template<class MultiTP = MultiTexturePack>
	std::unique_ptr<TexturePack> parseMultiTextureTexturePack(Game& game, const rapidjson::Value& elem)
	{
		using namespace std::literals;

		std::shared_ptr<Palette> palette;
		if (isValidString(elem, "palette") == true)
		{
			palette = game.Resources().getPalette(elem["palette"sv].GetStringView());
		}

		auto texturePack = std::make_unique<MultiTP>(palette);
		auto globalOffset = getVector2fKey<sf::Vector2f>(elem, "offset");

		for (const auto& val : elem["texture"sv])
		{
			if (isValidString(val, "id") == false)
			{
				continue;
			}
			TexturePackGroup t;
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
}
