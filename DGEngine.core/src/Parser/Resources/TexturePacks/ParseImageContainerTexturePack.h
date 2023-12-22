#pragma once

#include "Game/Game.h"
#include "Json/JsonParser.h"
#include <memory>
#include "Parser/Utils/ParseUtils.h"
#include "Resources/TexturePacks/ImageContainerTexturePack.h"
#include <type_traits>

namespace Parser
{
	std::vector<std::shared_ptr<ImageContainer>> getImageContainers(Game& game, const rapidjson::Value& elem);

	template<class ImageContainerTP = ImageContainerTexturePack>
	std::unique_ptr<TexturePack> parseImageContainerTexturePack(Game& game, const rapidjson::Value& elem)
	{
		static_assert(std::is_base_of_v<ImageContainerTexturePack, ImageContainerTP>);

		using namespace std::literals;

		auto imgContainers = getImageContainers(game, elem);
		if (imgContainers.empty() == true)
		{
			return nullptr;
		}

		std::shared_ptr<Palette> pal;
		if (isValidString(elem, "palette") == true)
		{
			pal = game.Resources().getPalette(elem["palette"sv].GetStringView());
		}

		bool useIndexedImages = pal != nullptr && game.Shaders().hasSpriteShader();
		auto offset = getVector2fKey<sf::Vector2f>(elem, "offset");

		return std::make_unique<ImageContainerTP>(
			imgContainers, offset, pal, useIndexedImages
		);
	}
}
