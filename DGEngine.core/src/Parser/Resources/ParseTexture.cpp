#include "ParseTexture.h"
#include <cassert>
#include "Game/Game.h"
#include "Game/Utils/ImageUtils.h"
#include "Hooks.h"
#include "Parser/ParseCommon.h"
#include "Parser/Utils/ParseUtils.h"
#include "ParseResource.h"
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

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
				for (unsigned j = 0; j < size.y; j++)
				{
					for (unsigned i = 0; i < size.x; i++)
					{
						img.setPixel(i, j, getColorVal(colorElem[i + j * size.x]));
					}
				}
			}
			else
			{
				img.create(size.x, size.y, getColorVal(colorElem));
			}
		}
		else if (elem.HasMember("file"sv) == true &&
			(Hooks::ParseTextureImg == nullptr ||
			Hooks::ParseTextureImg(game, elem, img) == false))
		{
			auto path = getStringViewVal(elem["file"sv]);
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

	void parseTextureF(Game& game, const Value& elem, const getTextureObjFuncPtr getTextureObjFunc)
	{
		assert(getTextureObjFunc != nullptr);

		if (parseResourceFromId<std::shared_ptr<sf::Texture>>(game.Resources(), elem) == true)
		{
			return;
		}

		auto id = parseValidIdOrFile(elem);
		if (id.empty() == true)
		{
			return;
		}

		auto texture = getTextureObjFunc(game, elem);
		if (texture == nullptr)
		{
			return;
		}
		game.Resources().addTexture(id, texture, getStringViewKey(elem, "resource"));
	}

	void parseTexture(Game& game, const Value& elem)
	{
		parseTextureF(game, elem, getTextureObj);
	}
}
