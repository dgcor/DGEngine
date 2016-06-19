#include "ParseCircle.h"
#include "Circle.h"
#include "GameUtils.h"
#include "ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseCircle(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}

		auto circle = std::make_shared<Circle>((float)getInt(elem, "radius"));

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
			circle->setTextureRect(getIntRect(elem, "textureRect", rect));
		}

		auto anchor = getAnchor(elem, "anchor");
		circle->setAnchor(anchor);
		auto pos = getVector2f<sf::Vector2f>(elem, "position");
		if (getBool(elem, "relativeCoords", true) == true)
		{
			auto size = circle->Size();
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.MinSize());
			if (game.StretchToFit() == false)
			{
				GameUtils::setAnchorPosSize(anchor, pos, size, game.MinSize(), game.WindowSize());
			}
		}
		circle->Position(pos);
		circle->Visible(getBool(elem, "visible", true));

		circle->setFillColor(getColor(elem, "color", sf::Color::White));
		circle->setOutlineColor(getColor(elem, "outlineColor", sf::Color::White));
		circle->setOutlineThickness((float)getUInt(elem, "outlineThickness"));

		game.Resources().addDrawable(elem["id"].GetString(), circle);
	}
}
