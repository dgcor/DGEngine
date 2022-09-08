#include "ParseLevelObjectActions.h"
#include "Game/Actions/ActLevelObject.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser2::Actions
{
	using namespace Parser;
	using namespace rapidjson;

	std::shared_ptr<Action> parseLevelObjDelete(const Value& elem)
	{
		return std::make_shared<ActLevelObjDelete>(
			getStringViewKey(elem, "level"),
			getStringViewKey(elem, "object"));
	}

	std::shared_ptr<Action> parseLevelObjDeleteByClass(const Value& elem)
	{
		return std::make_shared<ActLevelObjDeleteByClass>(
			getStringViewKey(elem, "level"),
			getStringViewKey(elem, "class"));
	}

	std::shared_ptr<Action> parseLevelObjExecuteAction(const Value& elem)
	{
		return std::make_shared<ActLevelObjExecuteAction>(
			getStringViewKey(elem, "level"),
			getStringViewKey(elem, "object"));
	}

	std::shared_ptr<Action> parseLevelObjSetOutline(const Value& elem)
	{
		return std::make_shared<ActLevelObjSetOutline>(
			getStringViewKey(elem, "level"),
			getColorKey(elem, "outline", sf::Color::Transparent),
			getColorKey(elem, "ignore", sf::Color::Transparent));
	}

	std::shared_ptr<Action> parseLevelObjSetPalette(const Value& elem)
	{
		return std::make_shared<ActLevelObjSetPalette>(
			getStringViewKey(elem, "level"),
			getStringViewKey(elem, "palette"),
			getColorKey(elem, "color", sf::Color::White));
	}
}
