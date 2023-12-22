#pragma once

#include "Parser/Resources/TexturePacks/ParseSingleTextureTexturePack.h"
#include "Resources/TexturePacks/MultiTexturePack.h"

namespace Parser
{
	template<class MultiTP = MultiTexturePack>
	std::unique_ptr<TexturePack> parseMultiTextureTexturePack(Game& game, const rapidjson::Value& elem)
	{
		static_assert(std::is_base_of_v<MultiTexturePack, MultiTP>);

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

			auto textureGroup = parseTextureGroup(game, val, "id");
			if (textureGroup.has_value() == false)
			{
				continue;
			}

			textureGroup->offset += globalOffset;

			texturePack->addTextureGroup(std::move(*textureGroup));
		}
		if (texturePack->size() == 0)
		{
			return nullptr;
		}
		return texturePack;
	}
}
