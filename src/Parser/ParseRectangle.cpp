#include "ParseRectangle.h"
#include "Game.h"
#include "GameUtils.h"
#include "Rectangle.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseRectangle(Game& game, const Value& elem)
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

		auto rectangle = std::make_shared<Rectangle>(getVector2fKey<sf::Vector2f>(elem, "size"));

		if (elem.HasMember("texture"))
		{
			auto texture = game.Resources().getTexture(elem["texture"].GetString());
			if (texture != nullptr)
			{
				rectangle->setTexture(texture.get());
			}
		}

		if (elem.HasMember("textureRect"))
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

		game.Resources().addDrawable(id, rectangle, getStringViewKey(elem, "resource"));
	}
}
