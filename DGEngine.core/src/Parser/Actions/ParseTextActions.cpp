#include "ParseTextActions.h"
#include "Game/Actions/ActText.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;
	using namespace std::literals;

	template <class T>
	std::shared_ptr<Action> parseSetTextHelper(const Value& elem)
	{
		std::shared_ptr<T> action;
		if (elem.HasMember("binding"sv) == true)
		{
			action = std::make_shared<T>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "format"),
				getStringVectorKey(elem, "binding"));
		}
		else if (elem.HasMember("query"sv) == true)
		{
			action = std::make_shared<T>(
				getStringViewKey(elem, "id"),
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
			action = std::make_shared<T>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "text"),
				textOp);
		}
		if (getBoolKey(elem, "replaceAll") == true)
		{
			action->ReplaceAll();
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

	std::shared_ptr<Action> parseTextAppendText(const Value& elem)
	{
		return parseSetTextHelper<ActTextAppendText>(elem);
	}

	std::shared_ptr<Action> parseTextSetColor(const Value& elem)
	{
		return std::make_shared<ActTextSetColor>(
			getStringViewKey(elem, "id"),
			getColorKey(elem, "color", sf::Color::White));
	}

	std::shared_ptr<Action> parseTextSetFont(const Value& elem)
	{
		return std::make_shared<ActTextSetFont>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "font"));
	}

	std::shared_ptr<Action> parseTextSetSpacing(const Value& elem)
	{
		auto action = std::make_shared<ActTextSetSpacing>(
			getStringViewKey(elem, "id"));
		if (elem.HasMember("horizontal"sv) == true)
		{
			action->setHorizontalSpaceOffset(getIntVal(elem["horizontal"sv]));
		}
		if (elem.HasMember("vertical"sv) == true)
		{
			action->setVerticalSpaceOffset(getIntVal(elem["vertical"sv]));
		}
		return action;
	}

	std::shared_ptr<Action> parseTextSetText(const Value& elem)
	{
		return parseSetTextHelper<ActTextSetText>(elem);
	}
}
