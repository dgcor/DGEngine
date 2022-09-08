#include "ParseVariableActions.h"
#include "Game/Actions/ActVariable.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Action> parseVariableAdd(const Value& elem)
	{
		return std::make_shared<ActVariableAdd>(
			getStringViewKey(elem, "key"),
			getVariableKey(elem, "value"));
	}

	std::shared_ptr<Action> parseVariableClear(const Value& elem)
	{
		return std::make_shared<ActVariableClear>(getStringViewKey(elem, "key"));
	}

	std::shared_ptr<Action> parseVariableSave(const Value& elem)
	{
		return std::make_shared<ActVariableSave>(
			getStringViewKey(elem, "file"),
			getStringVectorKey(elem, "vars"));
	}

	std::shared_ptr<Action> parseVariableSet(const Value& elem)
	{
		if (elem.HasMember("values"sv) == true &&
			elem["values"sv].IsObject() == true)
		{
			return std::make_shared<ActVariablesSet>(
				getVariables(elem["values"sv]),
				getBoolKey(elem, "resolveValue", true));
		}
		auto key = getStringViewKey(elem, "key");
		if (isValidId(key) == false)
		{
			return nullptr;
		}
		return std::make_shared<ActVariableSet>(
			key,
			getVariableKey(elem, "value"),
			getBoolKey(elem, "resolveValue", true));
	}

	std::shared_ptr<Action> parseVariableSetIfNull(const Value& elem)
	{
		auto key = getStringViewKey(elem, "key");
		if (isValidId(key) == false)
		{
			return nullptr;
		}
		return std::make_shared<ActVariableSetIfNull>(
			key,
			getVariableKey(elem, "value"),
			getBoolKey(elem, "resolveValue", true));
	}
}
