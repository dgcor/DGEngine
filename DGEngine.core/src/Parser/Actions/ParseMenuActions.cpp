#include "ParseMenuActions.h"
#include "Game/Actions/ActMenu.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;
	using namespace std::literals;

	template <class T>
	std::shared_ptr<Action> parseSetMenuTextHelper(const Value& elem)
	{
		auto index = getUIntKey(elem, "index");
		std::shared_ptr<T> action;
		if (elem.HasMember("binding"sv) == true)
		{
			action = std::make_shared<T>(
				getStringViewKey(elem, "id"),
				index,
				getStringViewKey(elem, "format"),
				getStringVectorKey(elem, "binding"));
		}
		else if (elem.HasMember("query"sv) == true)
		{
			action = std::make_shared<T>(
				getStringViewKey(elem, "id"),
				index,
				getStringViewKey(elem, "text"),
				getStringViewKey(elem, "query"));
		}
		else
		{
			auto textOp = TextUtils::TextOp::Replace;
			if (getBoolKey(elem, "set") == true)
			{
				textOp = TextUtils::TextOp::Set;
			}
			else if (getBoolKey(elem, "replaceAll") == true)
			{
				textOp = TextUtils::TextOp::ReplaceAll;
			}
			action = std::make_shared<T>(
				getStringViewKey(elem, "id"),
				index,
				getStringViewKey(elem, "text"),
				textOp);
		}
		if (getBoolKey(elem, "removeEmptyLines") == true)
		{
			action->RemoveEmptyLines();
		}
		if (getBoolKey(elem, "trim") == true)
		{
			action->Trim();
		}
		return action;
	}

	std::shared_ptr<Action> parseMenuAppendText(const Value& elem)
	{
		return parseSetMenuTextHelper<ActMenuAppendText>(elem);
	}

	std::shared_ptr<Action> parseMenuClick(const Value& elem)
	{
		return std::make_shared<ActMenuClick>(
			getStringViewKey(elem, "id"),
			getUIntKey(elem, "index"),
			getBoolKey(elem, "playSound", true));
	}

	std::shared_ptr<Action> parseMenuClickVisible(const Value& elem)
	{
		return std::make_shared<ActMenuClickVisible>(
			getStringViewKey(elem, "id"),
			getUIntKey(elem, "index"),
			getBoolKey(elem, "playSound", true));
	}

	std::shared_ptr<Action> parseMenuMoveScrollbar(const Value& elem)
	{
		return std::make_shared<ActMenuMoveScrollbar>(
			getStringViewKey(elem, "idMenu"),
			getStringViewKey(elem, "idScrollbar"),
			getStringViewKey(elem, "idAnchorTo"),
			getUIntKey(elem, "range"),
			getBoolKey(elem, "focus"));
	}

	std::shared_ptr<Action> parseMenuRightClick(const Value& elem)
	{
		return std::make_shared<ActMenuRightClick>(
			getStringViewKey(elem, "id"),
			getUIntKey(elem, "index"),
			getBoolKey(elem, "playSound", true));
	}

	std::shared_ptr<Action> parseMenuRightClickVisible(const Value& elem)
	{
		return std::make_shared<ActMenuRightClickVisible>(
			getStringViewKey(elem, "id"),
			getUIntKey(elem, "index"),
			getBoolKey(elem, "playSound", true));
	}

	std::shared_ptr<Action> parseMenuSetColor(const Value& elem)
	{
		return std::make_shared<ActMenuSetColor>(
			getStringViewKey(elem, "id"),
			getUIntKey(elem, "index"),
			getColorKey(elem, "color", sf::Color::White));
	}

	std::shared_ptr<Action> parseMenuSetFont(const Value& elem)
	{
		return std::make_shared<ActMenuSetFont>(
			getStringViewKey(elem, "id"),
			getUIntKey(elem, "index"),
			getStringViewKey(elem, "font"));
	}

	std::shared_ptr<Action> parseMenuSetIndex(const Value& elem)
	{
		return std::make_shared<ActMenuSetIndex>(
			getStringViewKey(elem, "id"),
			getVariableKey(elem, "index"),
			getBoolKey(elem, "focus"));
	}

	std::shared_ptr<Action> parseMenuSetText(const Value& elem)
	{
		return parseSetMenuTextHelper<ActMenuSetText>(elem);
	}
}
