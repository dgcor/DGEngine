#include "ParseTexture.h"
#include "ParseCelFile.h"
#include "CelUtils.h"
#include "ImageUtils.h"
#include "Utils.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	sf::Image parseTextureImg(Game& game, const rapidjson::Value& elem,
		size_t* numFramesX, size_t* numFramesY)
	{
		sf::Image img;

		if (elem.HasMember("fill"))
		{
			auto size = getVector2uKey(elem, "size", game.WindowTexSize());
			img.create(size.x, size.y, getColorKey(elem, "fill"));
			return img;
		}
		else if (elem.HasMember("file") == false)
		{
			return img;
		}
		if (elem.HasMember("palette"))
		{
			auto pal = game.Resources().getPalette(getStringVal(elem["palette"]));
			if (pal == nullptr)
			{
				return img;
			}
			auto celFile = parseCelFileObj(game, elem);
			if (celFile == nullptr)
			{
				return img;
			}
			if (elem.HasMember("charMapFile") == true)
			{
				img = CelUtils::loadBitmapFontImage(*celFile, elem["charMapFile"].GetString(), *pal);
			}
			else if (elem.HasMember("frame") == true)
			{
				auto frameIdx = getUIntVal(elem["frame"]);
				img = CelUtils::loadImageFrame(*celFile, *pal, frameIdx);
			}
			else if (numFramesX != nullptr && numFramesY != nullptr)
			{
				// construct cel already vertically splitted, if pieces exists
				(*numFramesX) = getUIntKey(elem, "pieces", 1);
				img = CelUtils::loadImage(*celFile, *pal, *numFramesX, *numFramesY);
			}
			else
			{
				img = CelUtils::loadImage(*celFile, *pal);
			}
		}
		else
		{
			img = ImageUtils::loadImage(
				getStringCharVal(elem["file"]),
				getColorKey(elem, "mask"));

			if (elem.HasMember("split"))
			{
				auto piecesX = getUIntKey(elem, "pieces", 1);

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

	bool parseTextureFromId(Game& game, const Value& elem)
	{
		if (isValidString(elem, "fromId") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				std::string fromId(elem["fromId"].GetString());
				std::string id(elem["id"].GetString());
				if (fromId != id && isValidId(id) == true)
				{
					auto obj = game.Resources().getTexture(fromId);
					if (obj != nullptr)
					{
						game.Resources().addTexture(id, obj);
					}
				}
			}
			return true;
		}
		return false;
	}

	void parseTexture(Game& game, const Value& elem)
	{
		if (parseTextureFromId(game, elem) == true)
		{
			return;
		}
		std::string id;
		if (isValidString(elem, "id") == true)
		{
			id = elem["id"].GetString();
		}
		else
		{
			if (isValidString(elem, "file") == false)
			{
				return;
			}
			std::string file(elem["file"].GetString());
			if (getIdFromFile(file, id) == false)
			{
				return;
			}
		}
		if (isValidId(id) == false)
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

		texture->setRepeated(getBoolKey(elem, "repeat", true));

		game.Resources().addTexture(id, texture);
	}
}
