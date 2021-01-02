#include "ParseCircle.h"
#include "Circle.h"
#include "Game.h"
#include "GameUtils.h"
#include "Panel.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseCircle(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}
		auto id = elem["id"sv].GetStringView();
		if (isValidId(id) == false)
		{
			return;
		}

		auto circle = std::make_shared<Circle>((float)getIntKey(elem, "radius"));

		if (isValidString(elem, "texture"))
		{
			auto texture = game.Resources().getTexture(elem["texture"sv].GetStringView());
			if (texture != nullptr)
			{
				circle->setTexture(texture.get());
			}
		}

		if (isValidString(elem, "textureRect"))
		{
			sf::IntRect rect(0, 0, game.DrawRegionSize().x, game.DrawRegionSize().y);
			circle->setTextureRect(getIntRectVal(elem["textureRect"sv], rect));
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

		bool manageObjDrawing = true;
		if (isValidString(elem, "panel") == true)
		{
			auto panelId = getStringViewVal(elem["panel"sv]);
			auto panel = game.Resources().getDrawable<Panel>(panelId);
			if (panel != nullptr)
			{
				panel->addDrawable(circle);
				manageObjDrawing = false;
			}
		}
		game.Resources().addDrawable(
			id, circle, manageObjDrawing, getStringViewKey(elem, "resource")
		);
	}
}
