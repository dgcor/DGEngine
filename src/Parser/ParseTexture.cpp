#include "ParseTexture.h"
#include "CelUtils.h"
#include "ImageUtils.h"
#include "ParseUtils.h"
#include "Utils.h"

namespace Parser
{
	using namespace rapidjson;

	sf::Image parseTextureImg(Game& game, const rapidjson::Value& elem,
		size_t* numFramesX, size_t* numFramesY)
	{
		sf::Image img;

		if (elem.HasMember("fill"))
		{
			auto size = getVector2u(elem, "size", game.WindowTexSize());
			img.create(size.x, size.y, getColor(elem, "fill"));
			return img;
		}
		else if (elem.HasMember("file") == false)
		{
			return img;
		}

		std::string fileName = getString(elem["file"]);

		if (elem.HasMember("palette"))
		{
			auto pal = game.Resources().getPalette(getString(elem["palette"]));
			if (pal == nullptr)
			{
				return img;
			}
			bool isCl2 = Utils::endsWith(fileName, "cl2");

			if (elem.HasMember("charMapFile"))
			{
				img = CelUtils::loadBitmapFontImage(fileName.c_str(), elem["charMapFile"].GetString(), *pal, isCl2);
			}
			else if (numFramesX != nullptr && numFramesY != nullptr)
			{
				// construct cel already vertically splitted, if pieces exists
				(*numFramesX) = getUInt(elem, "pieces", 1);
				img = CelUtils::loadImage(fileName.c_str(), *pal, isCl2, *numFramesX, *numFramesY);
			}
			else
			{
				img = CelUtils::loadImage(fileName.c_str(), *pal, isCl2);
			}
		}
		else
		{
			img = ImageUtils::loadImage(fileName.c_str(), getColor(elem, "mask"));

			if (elem.HasMember("split"))
			{
				auto piecesX = getUInt(elem, "pieces", 1);

				if (elem["split"].GetString() == std::string("horizontal"))
				{
					img = ImageUtils::splitImageHorizontal(img, piecesX);
				}
				else if (elem["split"].GetString() == std::string("vertical"))
				{
					img = ImageUtils::splitImageVertical(img, piecesX);
				}

				if (numFramesX != nullptr)
				{
					(*numFramesX) = piecesX;
				}
				if (numFramesY != nullptr)
				{
					(*numFramesY) = (*numFramesY) / piecesX;
				}
			}
		}

		return img;
	}

	void parseTexture(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}

		auto img = parseTextureImg(game, elem);
		auto imgSize = img.getSize();
		if (imgSize.x == 0 || imgSize.y == 0)
		{
			return;
		}
		auto texture = std::make_shared<sf::Texture>();
		if (texture->loadFromImage(img) == false)
		{
			return;
		}

		texture->setRepeated(true);

		game.Resources().addTexture(elem["id"].GetString(), texture);
	}
}
