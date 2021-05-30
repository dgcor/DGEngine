#include "ParseRectangle.h"
#include <cassert>
#include "Game.h"
#include "GameUtils.h"
#include "Panel.h"
#include "Rectangle.h"
#include "Utils/ParseUtils.h"

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
			rectangle->setTextureRect(getIntRectKey(elem, "textureRect", rect));
		}

		auto anchor = getAnchorKey(elem, "anchor");
		rectangle->setAnchor(anchor);
		auto size = rectangle->getSize();
		auto pos = getPositionKey(elem, "position", size, game.RefSize());
		if (getBoolKey(elem, "relativeCoords", true) == true &&
			game.RefSize() != game.DrawRegionSize())
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.DrawRegionSize());
			rectangle->Size(size);
		}
		rectangle->Position(pos);
		rectangle->Visible(getBoolKey(elem, "visible", true));

		rectangle->setFillColor(getColorKey(elem, "color", sf::Color::White));
		rectangle->setOutlineColor(getColorKey(elem, "outlineColor", sf::Color::White));
		rectangle->setOutlineThickness((float)getUIntKey(elem, "outlineThickness"));

		return rectangle;
	}

	void parseRectangle(Game& game, const Value& elem,
		const getRectangleObjFuncPtr getRectangleObjFunc)
	{
		assert(getRectangleObjFunc != nullptr);

		if (isValidString(elem, "id") == false)
		{
			return;
		}
		auto id = elem["id"sv].GetStringView();
		if (isValidId(id) == false)
		{
			return;
		}

		auto rectangle = getRectangleObjFunc(game, elem);
		if (rectangle == nullptr)
		{
			return;
		}

		bool manageObjDrawing = true;
		if (isValidString(elem, "panel") == true)
		{
			auto panelId = getStringViewVal(elem["panel"sv]);
			auto panel = game.Resources().getDrawable<Panel>(panelId);
			if (panel != nullptr)
			{
				panel->addDrawable(rectangle);
				manageObjDrawing = false;
			}
		}
		game.Resources().addDrawable(
			id, rectangle, manageObjDrawing, getStringViewKey(elem, "resource")
		);
	}

	void parseRectangle(Game& game, const Value& elem)
	{
		parseRectangle(game, elem, getRectangleObj);
	}
}
