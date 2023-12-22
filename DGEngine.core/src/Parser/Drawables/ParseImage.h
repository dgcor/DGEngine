#pragma once

#include "Game/Game.h"
#include "Json/JsonParser.h"
#include <memory>
#include "Parser/Utils/ParseUtils.h"
#include <type_traits>

class Game;
class Image;

namespace Parser
{
	// parses all image properties
	void parseImageProperties(Game& game, const rapidjson::Value& elem, Image& image);

	// parses color and outline properties
	void parseImageOutlineProperties(const rapidjson::Value& elem, Image& image);

	template<class Img = Image>
	std::shared_ptr<Img> getImageObj(Game& game, const rapidjson::Value& elem)
	{
		static_assert(std::is_base_of_v<Image, Img>);

		using namespace std::literals;

		std::shared_ptr<Img> image;

		if (isValidString(elem, "texture"))
		{
			auto texture = game.Resources().getTexture(elem["texture"sv].GetStringView());
			if (texture == nullptr)
			{
				return nullptr;
			}
			image = std::make_shared<Img>(*texture);
		}
		else if (isValidString(elem, "texturePack"))
		{
			auto texPack = game.Resources().getTexturePack(elem["texturePack"sv].GetStringView());
			if (texPack == nullptr)
			{
				return nullptr;
			}
			TextureInfoVar tiVar;
			if (texPack->get(getUIntKey(elem, "textureIndex"), tiVar) == false)
			{
				return nullptr;
			}
			if (std::holds_alternative<TextureInfo>(tiVar) == true)
			{
				image = std::make_shared<Img>(std::get<TextureInfo>(tiVar));
			}
			else
			{
				image = std::make_shared<Img>(std::get<std::vector<TextureInfo>>(tiVar));
			}
		}
		else
		{
			return nullptr;
		}

		parseImageProperties(game, elem, *image);

		return image;
	}

	void parseImage(Game& game, const rapidjson::Value& elem);
}
