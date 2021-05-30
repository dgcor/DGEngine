#include "ParseShape.h"
#include <cassert>
#include "Game.h"
#include "GameUtils.h"
#include "Panel.h"
#include "Shape.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Shape> getShapeObj(Game& game, const Value& elem)
	{
		auto shape = std::make_shared<Shape>(getPrimitiveTypeKey(elem, "type"));

		for (const auto& val : elem["vertices"sv])
		{
			sf::Vertex v;
			v.color = getColorKey(val, "color", sf::Color::White);
			v.position = getVector2fKey<sf::Vector2f>(val, "position");
			shape->addVertex(v);
		}

		if (shape->getPointCount() == 0)
		{
			return nullptr;
		}

		auto anchor = getAnchorKey(elem, "anchor");
		shape->setAnchor(anchor);
		auto size = shape->Size();
		auto pos = getPositionKey(elem, "position", size, game.RefSize());
		if (getBoolKey(elem, "relativeCoords", true) == true &&
			game.RefSize() != game.DrawRegionSize())
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.DrawRegionSize());
		}
		shape->Position(pos);
		shape->Size(size);
		shape->Visible(getBoolKey(elem, "visible", true));

		return shape;
	}

	void parseShape(Game& game, const Value& elem,
		const getShapeObjFuncPtr getShapeObjFunc)
	{
		assert(getShapeObjFunc != nullptr);

		if (isValidString(elem, "id") == false ||
			isValidArray(elem, "vertices") == false)
		{
			return;
		}
		auto id = elem["id"sv].GetStringView();
		if (isValidId(id) == false)
		{
			return;
		}

		auto shape = getShapeObjFunc(game, elem);
		if (shape == nullptr)
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
				panel->addDrawable(shape);
				manageObjDrawing = false;
			}
		}
		game.Resources().addDrawable(
			id, shape, manageObjDrawing, getStringViewKey(elem, "resource")
		);
	}

	void parseShape(Game& game, const Value& elem)
	{
		parseShape(game, elem, getShapeObj);
	}
}
