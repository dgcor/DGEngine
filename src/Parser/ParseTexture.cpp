#include "ParseTexture.h"
#include "ParseImageContainer.h"
#include "ImageUtils.h"
#include "Shaders.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;

	sf::Image parseTextureImg(Game& game, const Value& elem)
	{
		sf::Image img;
		parseTextureImg(game, elem, img, nullptr);
		return img;
	}

	bool parseTextureImg(Game& game, const Value& elem,
		sf::Image& img, ImageCache* cache)
	{
		sf::Image* imgPtr = &img;

		if (elem.HasMember("color") == true)
		{
			const auto& colorElem = elem["color"];
			auto size = getVector2uKey(elem, "size", game.DrawRegionSize());

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
			return false;
		}
		else if (elem.HasMember("file") == false)
		{
			return false;
		}

		auto path = getStringViewVal(elem["file"]);
		auto pathLower = Utils::toLower(path);

		if (Utils::endsWith(pathLower, ".cel") == true ||
			Utils::endsWith(pathLower, ".cl2") == true)
		{
			auto pal = game.Resources().getPalette(getStringKey(elem, "palette"));
			PaletteArray* palArray = nullptr;
			if (getBoolKey(elem, "indexed") == true &&
				Shaders::supportsPalettes() == true)
			{
				pal = nullptr;
			}
			if (pal != nullptr)
			{
				palArray = &pal->palette;
			}
			auto imgContainer = parseImageContainerObj(game, elem);
			if (imgContainer == nullptr)
			{
				return false;
			}
			if (elem.HasMember("charMapFile") == true)
			{
				img = ImageUtils::loadBitmapFontImage(
					*imgContainer, getStringViewVal(elem["charMapFile"]), pal);
			}
			else if (elem.HasMember("frame") == true)
			{
				auto frameIdx = getUIntVal(elem["frame"]);
				img = ImageUtils::loadImageFrame(*imgContainer, palArray, frameIdx);
			}
			else
			{
				img = ImageUtils::loadImage(*imgContainer, pal);
			}
		}
		else
		{
			auto mask = getColorKey(elem, "mask", sf::Color::Transparent);
			if (cache != nullptr)
			{
				if (cache->path != path)
				{
					cache->image = ImageUtils::loadImage(path, mask);
					cache->path = path;
				}
				imgPtr = &cache->image;
			}
			else
			{
				img = ImageUtils::loadImage(path, mask);
			}
			if (isValidString(elem, "split") == true)
			{
				auto split = getStringViewVal(elem["split"]);
				auto piecesX = getUIntKey(elem, "pieces", 1);

				if (split == "horizontal")
				{
					img = ImageUtils::splitImageHorizontal(*imgPtr, piecesX);
					imgPtr = &img;
				}
				else if (split == "vertical")
				{
					img = ImageUtils::splitImageVertical(*imgPtr, piecesX);
					imgPtr = &img;
				}
			}
		}
		if (isValidArray(elem, "trim") == true)
		{
			auto trimRect = getIntRectVal(elem["trim"]);
			auto imgSize = imgPtr->getSize();

			if (trimRect.left >= 0 &&
				trimRect.top >= 0 &&
				trimRect.width > 0 &&
				trimRect.height > 0 &&
				trimRect.left + trimRect.width <= (int)imgSize.x &&
				trimRect.top + trimRect.height <= (int)imgSize.y)
			{
				sf::Image img2;
				img2.create((unsigned)trimRect.width, (unsigned)trimRect.height);
				img2.copy(*imgPtr, 0, 0, trimRect);
				img = img2;
				return false;
			}
		}
		if (imgPtr != &img)
		{
			return true;
		}
		return false;
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
						game.Resources().addTexture(id, obj, getStringViewKey(elem, "resource"));
					}
				}
			}
			return true;
		}
		return false;
	}

	std::shared_ptr<sf::Texture> parseTextureObj(Game& game,
		const Value& elem, const sf::Image& image)
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

	std::shared_ptr<sf::Texture> parseTextureObj(Game& game,
		const Value& elem, ImageCache* cache)
	{
		sf::Image img;
		sf::Image* imgPtr = &img;
		if (parseTextureImg(game, elem, img, cache) == true)
		{
			imgPtr = &cache->image;
		}
		auto imgSize = imgPtr->getSize();
		if (imgSize.x == 0 || imgSize.y == 0)
		{
			return nullptr;
		}
		return parseTextureObj(game, elem, *imgPtr);
	}

	void parseTexture(Game& game, const Value& elem)
	{
		return parseTexture(game, elem, nullptr);
	}

	void parseTexture(Game& game, const Value& elem, ImageCache* cache)
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
			auto file = getStringViewVal(elem["file"]);
			if (getIdFromFile(file, id) == false)
			{
				return;
			}
		}
		if (isValidId(id) == false)
		{
			return;
		}
		auto texture = parseTextureObj(game, elem, cache);
		if (texture == nullptr)
		{
			return;
		}
		game.Resources().addTexture(id, texture, getStringViewKey(elem, "resource"));
	}

	bool getOrParseTexture(Game& game, const Value& elem, const char* idKey,
		std::shared_ptr<sf::Texture>& texture)
	{
		return getOrParseTexture(game, elem, idKey, texture, nullptr);
	}

	bool getOrParseTexture(Game& game, const Value& elem, const char* idKey,
		std::shared_ptr<sf::Texture>& texture, ImageCache* cache)
	{
		if (isValidString(elem, idKey) == true)
		{
			std::string id = elem[idKey].GetString();
			texture = game.Resources().getTexture(id);
			if (texture != nullptr)
			{
				return true;
			}
			texture = parseTextureObj(game, elem, cache);
			if (isValidId(id) == true &&
				texture != nullptr)
			{
				game.Resources().addTexture(id, texture, getStringViewKey(elem, "resource"));
				return true;
			}
		}
		else
		{
			texture = parseTextureObj(game, elem, cache);
		}
		return false;
	}
}
