#include "ParseTexture.h"
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
			img.create(size.x, size.y, getColorVar(game, elem, "fill"));
			return img;
		}
		else if (elem.HasMember("file") == false)
		{
			return img;
		}

		std::string fileName = getStringVal(elem["file"]);

		if (elem.HasMember("palette"))
		{
			auto pal = game.Resources().getPalette(getStringVal(elem["palette"]));
			if (pal == nullptr)
			{
				return img;
			}
			bool isCl2 = Utils::endsWith(fileName, "cl2");

			if (elem.HasMember("charMapFile") == true)
			{
				img = CelUtils::loadBitmapFontImage(fileName.c_str(), elem["charMapFile"].GetString(), *pal, isCl2);
			}
			else if (elem.HasMember("frame") == true)
			{
				auto frameIdx = getUIntVal(elem["frame"]);
				img = CelUtils::loadImageFrame(fileName.c_str(), *pal, isCl2, frameIdx);
			}
			else if (numFramesX != nullptr && numFramesY != nullptr)
			{
				// construct cel already vertically splitted, if pieces exists
				(*numFramesX) = getUIntKey(elem, "pieces", 1);
				img = CelUtils::loadImage(fileName.c_str(), *pal, isCl2, *numFramesX, *numFramesY);
			}
			else
			{
				img = CelUtils::loadImage(fileName.c_str(), *pal, isCl2);
			}
		}
		else
		{
			img = ImageUtils::loadImage(fileName.c_str(), getColorVar(game, elem, "mask"));

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
