#pragma once

#include "Game/Game.h"
#include "Json/JsonParser.h"
#include <memory>
#include "Parser/Utils/ParseUtils.h"
#include "Resources/TexturePacks/ImageContainerTexturePack.h"
#include "Resources/TexturePacks/MultiImageContainerTexturePack.h"

namespace Parser
{
	std::vector<std::shared_ptr<ImageContainer>> getImageContainers(Game& game, const rapidjson::Value& elem);

	template<class ImageContainerTP = ImageContainerTexturePack, class MultiImageContainerTP = MultiImageContainerTexturePack>
	std::unique_ptr<TexturePack> parseImageContainerTexturePack(Game& game, const rapidjson::Value& elem)
	{
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

		if (imgContainers.size() == 1)
		{
			return std::make_unique<ImageContainerTP>(
				imgContainers.front(), offset, pal, useIndexedImages
			);
		}
		else
		{
			return std::make_unique<MultiImageContainerTP>(
				imgContainers, offset, pal, useIndexedImages
			);
		}
	}
}
