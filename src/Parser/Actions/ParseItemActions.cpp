#include "ParseItemActions.h"
#include "Game/Actions/ActItem.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"
#include "Parser/Utils/ParseUtilsGameKey.h"

namespace Parser2::Actions
{
	using namespace Parser;
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Action> parseItemAddCursor(const Value& elem)
	{
		return std::make_shared<ActItemAddCursor>(
			getStringViewKey(elem, "level"),
			getItemLocationVal(elem));
	}

	std::shared_ptr<Action> parseItemAddQuantity(const Value& elem)
	{
		return std::make_shared<ActItemAddQuantity>(
			getStringViewKey(elem, "level"),
			getItemLocationVal(elem),
			getVariableKey(elem, "value"),
			getBoolKey(elem, "remove"));
	}

	std::shared_ptr<Action> parseItemDelete(const Value& elem)
	{
		return std::make_shared<ActItemDelete>(
			getStringViewKey(elem, "level"),
			getItemLocationVal(elem));
	}

	std::shared_ptr<Action> parseItemDrop(const Value& elem)
	{
		return std::make_shared<ActItemDrop>(
			getStringViewKey(elem, "level"),
			getItemCoordInventoryVal(elem));
	}

	std::shared_ptr<Action> parseItemExecuteAction(const Value& elem)
	{
		return std::make_shared<ActItemExecuteAction>(
			getStringViewKey(elem, "level"),
			getItemLocationVal(elem),
			str2int16(getStringViewKey(elem, "action")));
	}

	std::shared_ptr<Action> parseItemLoadFromLevel(Game& game, const Value& elem)
	{
		auto action = std::make_shared<ActItemLoadFromLevel>(
			getStringViewKey(elem, "level"),
			getItemCoordInventoryVal(elem),
			getBoolKey(elem, "splitIntoMultiple", true));

		if (elem.HasMember("position"sv) == true)
		{
			action->setInventoryPosition(getInventoryPositionVal(elem["position"sv]));
		}
		if (elem.HasMember("onInventoryFull"sv) == true)
		{
			action->setInventoryFullAction(getActionVal(game, elem["onInventoryFull"sv]));
		}
		return action;
	}

	std::shared_ptr<Action> parseItemMove(const Value& elem)
	{
		return std::make_shared<ActItemMove>(
			getStringViewKey(elem, "level"),
			getItemLocationKey(elem, "from"),
			getItemLocationKey(elem, "to"));
	}

	std::shared_ptr<Action> parseItemSetProperty(const Value& elem)
	{
		return std::make_shared<ActItemSetProperty>(
			getItemLocationVal(elem),
			getStringViewKey(elem, "level"),
			getStringViewKey(elem, "property"),
			getVariableKey(elem, "value"));
	}

	std::shared_ptr<Action> parseItemTrade(Game& game, const Value& elem)
	{
		auto action = std::make_shared<ActItemTrade>(
			getStringViewKey(elem, "level"),
			getStringViewKey(elem, "player"),
			getItemCoordInventoryKey(elem, "item"),
			getInventoryPositionKey(elem, "position"));

		if (elem.HasMember("onInventoryFull"sv) == true)
		{
			action->setInventoryFullAction(getActionVal(game, elem["onInventoryFull"sv]));
		}
		return action;
	}

	std::shared_ptr<Action> parseItemUpdate(const Value& elem)
	{
		return std::make_shared<ActItemUpdate>(
			getStringViewKey(elem, "level"),
			getItemCoordInventoryVal(elem));
	}

	std::shared_ptr<Action> parseItemUse(const Value& elem)
	{
		return std::make_shared<ActItemUse>(
			getStringViewKey(elem, "level"),
			getItemCoordInventoryVal(elem));
	}
}
