#include "ParseResourceActions.h"
#include "Game/Actions/ActResource.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Action> parseResourceAdd(const Value& elem)
	{
		auto id = getStringViewKey(elem, "id");
		if (isValidId(id) == false)
		{
			return nullptr;
		}
		auto action = std::make_shared<ActResourceAdd>(id);
		if (elem.HasMember("ignorePrevious"sv) == true)
		{
			action->setIgnorePrevious(
				getIgnoreResourceVal(elem["ignorePrevious"sv]));
		}
		return action;
	}

	std::shared_ptr<Action> parseResourceBringToFront(const Value& elem)
	{
		return std::make_shared<ActResourceBringToFront>(getStringViewKey(elem, "id"));
	}

	std::shared_ptr<Action> parseResourceIgnore(const Value& elem)
	{
		return std::make_shared<ActResourceIgnore>(
			getStringViewKey(elem, "id"),
			getIgnoreResourceKey(elem, "ignore", IgnoreResource::Draw | IgnoreResource::Update));
	}

	std::shared_ptr<Action> parseResourcePop(const Value& elem)
	{
		return std::make_shared<ActResourcePop>(
			getStringViewKey(elem, "id"),
			getIgnoreResourceKey(elem, "ignorePrevious"));
	}

	std::shared_ptr<Action> parseResourcePopAll(const Value& elem)
	{
		return std::make_shared<ActResourcePopAll>(
			getStringViewKey(elem, "id"),
			getBoolKey(elem, "popBase"),
			getIgnoreResourceKey(elem, "ignorePrevious"));
	}
}
