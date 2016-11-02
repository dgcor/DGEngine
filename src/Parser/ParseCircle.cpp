#include "ParseCircle.h"
#include "Circle.h"
#include "GameUtils.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseCircle(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}
		std::string id(elem["id"].GetString());
		if (isValidId(id) == false)
		{
			return;
		}

		auto circle = std::make_shared<Circle>((float)getIntKey(elem, "radius"));

		if (elem.HasMember("texture"))
		{
			auto texture = game.Resources().getTexture(elem["texture"].GetString());
			if (texture != nullptr)
			{
				circle->setTexture(texture.get());
			}
		}

		if (elem.HasMember("textureRect"))
		{
			sf::IntRect rect(0, 0, game.WindowSize().x, game.WindowSize().y);
			circle->setTextureRect(getIntRectKey(elem, "textureRect", rect));
		}

		auto anchor = getAnchorKey(elem, "anchor");
		circle->setAnchor(anchor);
		auto pos = getVector2fKey<sf::Vector2f>(elem, "position");
		if (getBoolKey(elem, "relativeCoords", true) == true)
		{
			auto size = circle->Size();
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.MinSize());
			if (game.StretchToFit() == false)
			{
				GameUtils::setAnchorPosSize(anchor, pos, size, game.MinSize(), game.WindowSize());
			}
		}
		circle->Position(pos);
		circle->Visible(getBoolKey(elem, "visible", true));

		circle->setFillColor(getColorVar(game, elem, "color", sf::Color::White));
		circle->setOutlineColor(getColorVar(game, elem, "outlineColor", sf::Color::White));
		circle->setOutlineThickness((float)getUIntKey(elem, "outlineThickness"));

		game.Resources().addDrawable(id, circle);
	}
}
