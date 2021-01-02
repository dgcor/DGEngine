#include "ParseTexture.h"
#include "Game.h"
#ifndef NO_DIABLO_FORMAT_SUPPORT
#include "ImageContainers/DC6ImageContainer.h"
#endif
#include "ImageUtils.h"
#include "ParseImageContainer.h"
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	sf::Image parseTextureImg(Game& game, const Value& elem)
	{
		sf::Image img;

		if (elem.HasMember("color"sv) == true)
		{
			const auto& colorElem = elem["color"sv];
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
			return img;
		}
		else if (elem.HasMember("file"sv) == false)
		{
			return img;
		}

		auto path = getStringViewVal(elem["file"sv]);
		auto pathLower = Utils::toLower(path);

		if (Utils::endsWith(pathLower, ".cel") == true ||
			Utils::endsWith(pathLower, ".cl2") == true ||
			Utils::endsWith(pathLower, ".dc6") == true)
		{
			auto pal = game.Resources().getPalette(getStringKey(elem, "palette"));
			PaletteArray* palArray = nullptr;
			if (getBoolKey(elem, "indexed") == true &&
				game.Shaders().hasSpriteShader() == true)
			{
				pal = nullptr;
			}
			if (pal != nullptr)
			{
				palArray = &pal->palette;
			}
			auto imgContainer = getImageContainerObj(game, elem);
			if (imgContainer == nullptr)
			{
				return img;
			}
			if (elem.HasMember("charMapFile"sv) == true)
			{
				img = ImageUtils::loadBitmapFontImage(
					*imgContainer, getStringViewVal(elem["charMapFile"sv]), pal);
			}
			else if (elem.HasMember("frame"sv) == true)
			{
				auto frameIdx = getUIntVal(elem["frame"sv]);
				img = ImageUtils::loadImageFrame(*imgContainer, palArray, frameIdx);
			}
#ifndef NO_DIABLO_FORMAT_SUPPORT
			else if (elem.HasMember("stitch"sv) == true &&
				elem.HasMember("size"sv) == true &&
				dynamic_cast<DC6ImageContainer*>(imgContainer.get()) != nullptr)
			{
				auto dc6ImgCont = dynamic_cast<DC6ImageContainer*>(imgContainer.get());
				auto stitch = getVector2uVal<sf::Vector2u>(elem["stitch"sv]);
				auto size = getVector2uVal<sf::Vector2u>(elem["size"sv]);
				img = dc6ImgCont->get(stitch, size, palArray);
			}
#endif
			else
			{
				img = ImageUtils::loadImage(*imgContainer, pal);
			}
		}
		else
		{
			auto mask = getColorKey(elem, "mask", sf::Color::Transparent);
			img = ImageUtils::loadImage(path, mask);
			if (isValidString(elem, "split") == true)
			{
				auto split = getStringViewVal(elem["split"sv]);
				auto piecesX = getUIntKey(elem, "pieces", 1);

				if (split == "horizontal")
				{
					img = ImageUtils::splitImageHorizontal(img, piecesX);
				}
				else if (split == "vertical")
				{
					img = ImageUtils::splitImageVertical(img, piecesX);
				}
			}
		}
		if (isValidArray(elem, "trim") == true)
		{
			auto trimRect = getIntRectVal(elem["trim"sv]);
			auto imgSize = img.getSize();

			if (trimRect.left >= 0 &&
				trimRect.top >= 0 &&
				trimRect.width > 0 &&
				trimRect.height > 0 &&
				trimRect.left + trimRect.width <= (int)imgSize.x &&
				trimRect.top + trimRect.height <= (int)imgSize.y)
			{
				sf::Image img2;
				img2.create((unsigned)trimRect.width, (unsigned)trimRect.height);
				img2.copy(img, 0, 0, trimRect);
				img = std::move(img2);
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
				auto fromId = elem["fromId"sv].GetStringView();
				auto id = elem["id"sv].GetStringView();
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

	std::shared_ptr<sf::Texture> getTextureObj(Game& game, const Value& elem)
	{
		auto img = parseTextureImg(game, elem);
		auto imgSize = img.getSize();
		if (imgSize.x == 0 || imgSize.y == 0)
		{
			return nullptr;
		}
		auto texture = std::make_shared<sf::Texture>();
		if (texture->loadFromImage(img) == false)
		{
			return nullptr;
		}
		texture->setRepeated(getBoolKey(elem, "repeat", true));
		texture->setSmooth(getBoolKey(elem, "smooth", true));
		return texture;
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
			id = elem["id"sv].GetStringView();
		}
		else
		{
			if (isValidString(elem, "file") == false)
			{
				return;
			}
			auto file = getStringViewVal(elem["file"sv]);
			if (getIdFromFile(file, id) == false)
			{
				return;
			}
		}
		if (isValidId(id) == false)
		{
			return;
		}
		auto texture = getTextureObj(game, elem);
		if (texture == nullptr)
		{
			return;
		}
		game.Resources().addTexture(id, texture, getStringViewKey(elem, "resource"));
	}
}
