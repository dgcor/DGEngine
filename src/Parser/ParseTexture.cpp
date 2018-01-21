#include "ParseTexture.h"
#include "ParseImageContainer.h"
#include "ImageUtils.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;

	sf::Image parseTextureImg(Game& game, const rapidjson::Value& elem)
	{
		sf::Image img;

		if (elem.HasMember("color") == true)
		{
			const auto& colorElem = elem["color"];
			auto size = getVector2uKey(elem, "size", game.WindowTexSize());

			if (colorElem.IsArray() == true &&
				colorElem.Size() == (size.x * size.y))
			{
				img.create(size.x, size.y);
				for (size_t j = 0; j < size.y; j++)
				{
					for (size_t i = 0; i < size.x; i++)
					{
						img.setPixel(i, j, getColorVal(colorElem[i + j * size.y]));
					}
				}
			}
			else
			{
				img.create(size.x, size.y, getColorVal(colorElem));
			}
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
			auto imgContainer = parseImageContainerObj(game, elem);
			if (imgContainer == nullptr)
			{
				return img;
			}
			if (elem.HasMember("charMapFile") == true)
			{
				img = ImageUtils::loadBitmapFontImage(
					*imgContainer, elem["charMapFile"].GetString(), pal);
			}
			else if (elem.HasMember("frame") == true)
			{
				auto frameIdx = getUIntVal(elem["frame"]);
				img = ImageUtils::loadImageFrame(*imgContainer, *pal, frameIdx);
			}
			else
			{
				img = ImageUtils::loadImage(*imgContainer, pal);
			}
		}
		else
		{
			img = ImageUtils::loadImage(
				getStringCharVal(elem["file"]),
				getColorKey(elem, "mask", sf::Color::Transparent));

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

	std::shared_ptr<sf::Texture> parseTextureObj(Game& game,
		const rapidjson::Value& elem, const sf::Image& image)
	{
		auto texture = std::make_shared<sf::Texture>();
		if (texture->loadFromImage(image) == false)
		{
			return nullptr;
		}
		texture->setRepeated(getBoolKey(elem, "repeat", true));
		return texture;
	}

	std::shared_ptr<sf::Texture> parseTextureObj(Game& game, const Value& elem)
	{
		auto img = parseTextureImg(game, elem);
		auto imgSize = img.getSize();
		if (imgSize.x == 0 || imgSize.y == 0)
		{
			return nullptr;
		}
		return parseTextureObj(game, elem, img);
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
		auto texture = parseTextureObj(game, elem);
		if (texture == nullptr)
		{
			return;
		}
		game.Resources().addTexture(id, texture);
	}

	bool getOrParseTexture(Game& game, const Value& elem,
		const char* idKey, std::shared_ptr<sf::Texture>& texture)
	{
		if (isValidString(elem, idKey) == true)
		{
			std::string id = elem[idKey].GetString();
			texture = game.Resources().getTexture(id);
			if (texture != nullptr)
			{
				return true;
			}
			texture = parseTextureObj(game, elem);
			if (isValidId(id) == true &&
				texture != nullptr)
			{
				game.Resources().addTexture(id, texture);
				return true;
			}
		}
		else
		{
			texture = parseTextureObj(game, elem);
		}
		return false;
	}
}
