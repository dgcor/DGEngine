#include "ParseMountActions.h"
#include "Game/Actions/ActMount.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;

	std::shared_ptr<Action> parseMount(const Value& elem)
	{
		return std::make_shared<ActMount>(
			getStringViewKey(elem, "file"),
			getStringViewKey(elem, "mount"),
			getBoolKey(elem, "append", true),
			getBoolKey(elem, "useSaveDir"));
	}

	std::shared_ptr<Action> parseUnmount(const Value& elem)
	{
		return std::make_shared<ActUnmount>(
			getStringViewKey(elem, "file"),
			getBoolKey(elem, "useSaveDir"));
	}

	std::shared_ptr<Action> parseUnmountAll()
	{
		return std::make_shared<ActUnmountAll>();
	}
}
