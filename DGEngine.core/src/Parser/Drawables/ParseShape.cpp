#include "ParseShape.h"
#include "Game/Drawables/Shape.h"
#include "Game/Game.h"
#include "ParseDrawable.h"
#include "Parser/Utils/ParseUtils.h"

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

		parseDrawableProperties(game, elem, *shape);

		return shape;
	}

	static std::shared_ptr<UIObject> parseShapeFunc(Game& game, const Value& elem)
	{
		return getShapeObj(game, elem);
	}

	void parseShape(Game& game, const Value& elem)
	{
		if (isValidArray(elem, "vertices") == false)
		{
			return;
		}
		parseValidIdAndAddDrawable(game, elem, parseShapeFunc);
	}
}
