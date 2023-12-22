#pragma once

#include "Game/Game.h"
#include "Json/JsonParser.h"
#include <memory>
#include <optional>
#include "Parser/Utils/ParseUtils.h"
#include "Resources/TexturePacks/MultiTexturePack.h"
#include <type_traits>
#include <vector>

namespace Parser
{
	std::optional<TextureGroup> parseTextureGroup(const Game& game,
		const rapidjson::Value& elem, const std::string_view textureKey);

	template<class MultiTP = MultiTexturePack>
	std::unique_ptr<TexturePack> parseSingleTextureTexturePack(Game& game, const rapidjson::Value& elem)
	{
		static_assert(std::is_base_of_v<MultiTexturePack, MultiTP>);

		using namespace std::literals;

		if (isValidString(elem, "texture") == false)
		{
			return nullptr;
		}

		auto textureGroup = parseTextureGroup(game, elem, "texture");
		if (textureGroup.has_value() == false)
		{
			return nullptr;
		}

		std::shared_ptr<Palette> palette;
		if (isValidString(elem, "palette") == true)
		{
			palette = game.Resources().getPalette(elem["palette"sv].GetStringView());
		}

		auto texturePack = std::make_unique<MultiTP>(std::move(*textureGroup), palette);

		if (texturePack->size() == 0)
		{
			return nullptr;
		}
		return texturePack;
	}
}
