#include "ParseCircle.h"
#include "Game/Drawables/Circle.h"
#include "Game/Game.h"
#include "ParseDrawable.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Circle> getCircleObj(Game& game, const Value& elem)
	{
		auto circle = std::make_shared<Circle>((float)getIntKey(elem, "radius"));

		if (isValidString(elem, "texture"))
		{
			auto texture = game.Resources().getTexture(elem["texture"sv].GetStringView());
			if (texture != nullptr)
			{
				circle->setTexture(texture.get());
			}
		}

		if (elem.HasMember("textureRect"sv))
		{
			sf::IntRect rect(0, 0, game.DrawRegionSize().x, game.DrawRegionSize().y);
			circle->setTextureRect(getIntRectVal(elem["textureRect"sv], rect));
		}

		parseDrawableProperties(game, elem, *circle);

		circle->setFillColor(getColorKey(elem, "color", sf::Color::White));
		circle->setOutlineColor(getColorKey(elem, "outlineColor", sf::Color::White));
		circle->setOutlineThickness((float)getUIntKey(elem, "outlineThickness"));

		return circle;
	}

	static std::shared_ptr<UIObject> parseCircleFunc(Game& game, const Value& elem)
	{
		return getCircleObj(game, elem);
	}

	void parseCircle(Game& game, const Value& elem)
	{
		parseValidIdAndAddDrawable(game, elem, parseCircleFunc);
	}
}
