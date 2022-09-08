#include "ParseImageActions.h"
#include "Game/Actions/ActImage.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Action> parseImageEnableOutline(const Value& elem)
	{
		return std::make_shared<ActImageEnableOutline>(
			getStringViewKey(elem, "id"),
			getBoolKey(elem, "enable", true));
	}

	std::shared_ptr<Action> parseImageInverseResize(const Value& elem, bool applyToY)
	{
		return std::make_shared<ActImageInverseResizeXY>(
			getStringViewKey(elem, "id"),
			getVariableKey(elem, "size"),
			getVariableKey(elem, "inputRangeMin"),
			getVariableKey(elem, "inputRangeMax"),
			getVector2iKey<sf::Vector2i>(elem, "range"),
			applyToY);
	}

	std::shared_ptr<Action> parseImageInverseResizeX(const Value& elem)
	{
		return parseImageInverseResize(elem, false);
	}

	std::shared_ptr<Action> parseImageInverseResizeY(const Value& elem)
	{
		return parseImageInverseResize(elem, true);
	}

	std::shared_ptr<Action> parseImageSetOutline(const Value& elem)
	{
		return std::make_shared<ActImageSetOutline>(
			getStringViewKey(elem, "id"),
			getColorKey(elem, "outline", sf::Color::Transparent),
			getColorKey(elem, "ignore", sf::Color::Transparent));
	}

	std::shared_ptr<Action> parseImageSetPalette(const Value& elem)
	{
		return std::make_shared<ActImageSetPalette>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "palette"),
			getColorKey(elem, "color", sf::Color::White));
	}

	std::shared_ptr<Action> parseImageSetTexture(const Value& elem)
	{
		if (elem.HasMember("texture"sv) == true)
		{
			return std::make_shared<ActImageSetTexture>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "texture"),
				getBoolKey(elem, "resetRect"));
		}
		else if (elem.HasMember("texturePack"sv) == true)
		{
			return std::make_shared<ActImageSetTextureFromPack>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "texturePack"),
				getUIntKey(elem, "index"));
		}
		else
		{
			return std::make_shared<ActImageSetTextureFromQueryable>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "query"),
				getUIntKey(elem, "index"));
		}
	}

	std::shared_ptr<Action> parseImageSetTextureRect(const Value& elem)
	{
		return std::make_shared<ActImageSetTextureRect>(
			getStringViewKey(elem, "id"),
			getIntRectKey(elem, "rect"));
	}
}
