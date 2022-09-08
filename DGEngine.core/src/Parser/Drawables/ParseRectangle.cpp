#include "ParseRectangle.h"
#include "Game/Drawables/Rectangle.h"
#include "Game/Game.h"
#include "ParseDrawable.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Rectangle> getRectangleObj(Game& game, const Value& elem)
	{
		auto rectangle = std::make_shared<Rectangle>(getVector2fKey<sf::Vector2f>(elem, "size"));

		if (isValidString(elem, "texture"))
		{
			auto texture = game.Resources().getTexture(elem["texture"sv].GetStringView());
			if (texture != nullptr)
			{
				rectangle->setTexture(texture.get());
			}
		}

		if (elem.HasMember("textureRect"sv))
		{
			sf::IntRect rect(0, 0, game.DrawRegionSize().x, game.DrawRegionSize().y);
			rectangle->setTextureRect(getIntRectVal(elem["textureRect"sv], rect));
		}

		parseDrawableProperties(game, elem, *rectangle);

		rectangle->setFillColor(getColorKey(elem, "color", sf::Color::White));
		rectangle->setOutlineColor(getColorKey(elem, "outlineColor", sf::Color::White));
		rectangle->setOutlineThickness((float)getUIntKey(elem, "outlineThickness"));

		return rectangle;
	}

	static std::shared_ptr<UIObject> parseRectangleFunc(Game& game, const Value& elem)
	{
		return getRectangleObj(game, elem);
	}

	void parseRectangle(Game& game, const Value& elem)
	{
		parseValidIdAndAddDrawable(game, elem, parseRectangleFunc);
	}
}
