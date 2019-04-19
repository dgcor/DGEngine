#include "ParseCircle.h"
#include "Circle.h"
#include "Game.h"
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
			sf::IntRect rect(0, 0, game.DrawRegionSize().x, game.DrawRegionSize().y);
			circle->setTextureRect(getIntRectKey(elem, "textureRect", rect));
		}

		auto anchor = getAnchorKey(elem, "anchor");
		circle->setAnchor(anchor);
		auto size = circle->Size();
		auto pos = getPositionKey(elem, "position", size, game.RefSize());
		if (getBoolKey(elem, "relativeCoords", true) == true &&
			game.RefSize() != game.DrawRegionSize())
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.DrawRegionSize());
		}
		circle->Position(pos);
		circle->Visible(getBoolKey(elem, "visible", true));

		circle->setFillColor(getColorKey(elem, "color", sf::Color::White));
		circle->setOutlineColor(getColorKey(elem, "outlineColor", sf::Color::White));
		circle->setOutlineThickness((float)getUIntKey(elem, "outlineThickness"));

		game.Resources().addDrawable(id, circle, getStringViewKey(elem, "resource"));
	}
}
