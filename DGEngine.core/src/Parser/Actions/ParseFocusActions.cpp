#include "ParseFocusActions.h"
#include "Game/Actions/ActFocus.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;

	std::shared_ptr<Action> parseFocusAdd(const rapidjson::Value& elem)
	{
		return std::make_shared<ActFocusAdd>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "resource"),
			getBoolKey(elem, "focus", true));
	}

	std::shared_ptr<Action> parseFocusClick(const rapidjson::Value& elem)
	{
		return std::make_shared<ActFocusClick>(getBoolKey(elem, "playSound", true));
	}

	std::shared_ptr<Action> parseFocusMoveDown()
	{
		return std::make_shared<ActFocusMoveDown>();
	}

	std::shared_ptr<Action> parseFocusMoveUp()
	{
		return std::make_shared<ActFocusMoveUp>();
	}

	std::shared_ptr<Action> parseFocusRightClick(const rapidjson::Value& elem)
	{
		return std::make_shared<ActFocusRightClick>(getBoolKey(elem, "playSound", true));
	}

	std::shared_ptr<Action> parseFocusSet(const rapidjson::Value& elem)
	{
		return std::make_shared<ActFocusSet>(
			getStringViewKey(elem, "id"),
			getBoolKey(elem, "focus", true));
	}

	std::shared_ptr<Action> parseFocusUpdate(const rapidjson::Value& elem)
	{
		return std::make_shared<ActFocusUpdate>();
	}
}
