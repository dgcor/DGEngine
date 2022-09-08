#pragma once

#include "Game/Game.h"
#include "Json/JsonParser.h"
#include <memory>
#include "Parser/Utils/ParseUtils.h"
#include "Resources/TexturePacks/SimpleTexturePack.h"
#include "Resources/TexturePacks/SingleTexturePack.h"
#include <vector>

namespace Parser
{
	void parseTexturePackDirectionVector(const rapidjson::Value& elem, uint32_t& directions,
		std::vector<std::pair<uint32_t, uint32_t>>& directionsVec);

	template<class SingleTP = SingleTexturePack, class SimpleTP = SimpleTexturePack>
	std::unique_ptr<TexturePack> parseSingleTextureTexturePack(Game& game, const rapidjson::Value& elem)
	{
		using namespace std::literals;

		if (isValidString(elem, "texture") == false)
		{
			return nullptr;
		}
		TexturePackGroup t;
		t.texture = game.Resources().getTexture(elem["texture"sv].GetStringView());
		if (t.texture == nullptr)
		{
			return nullptr;
		}
		std::shared_ptr<Palette> palette;
		if (isValidString(elem, "palette") == true)
		{
			palette = game.Resources().getPalette(elem["palette"sv].GetStringView());
		}

		auto frames = getFramesKey(elem, "frames");
		if (frames.first == 0 || frames.second == 0)
		{
			frames.first = frames.second = 1;
		}
		t.offset = getVector2fKey<sf::Vector2f>(elem, "offset");
		t.startIndex = getUIntKey(elem, "startIndex");
		parseTexturePackDirectionVector(elem, t.directions, t.directionsVec);
		t.horizontalDirection = getStringViewKey(elem, "direction") == "horizontal";
		t.animType = getAnimationTypeKey(elem, "animationType");
		t.refresh = getTimeKey(elem, "refresh");

		std::unique_ptr<TexturePack> texturePack;

		if (frames.first == 1 &&
			frames.second == 1 &&
			t.startIndex == 0)
		{
			texturePack = std::make_unique<SimpleTP>(std::move(t), palette);
		}
		else
		{
			texturePack = std::make_unique<SingleTP>(std::move(t), frames, palette);
		}

		if (texturePack->size() == 0)
		{
			return nullptr;
		}
		return texturePack;
	}
}
