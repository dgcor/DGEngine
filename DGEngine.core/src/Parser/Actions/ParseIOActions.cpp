#include "ParseIOActions.h"
#include "Game/Actions/ActIO.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;

	std::shared_ptr<Action> parseDirCopy(const Value& elem)
	{
		return std::make_shared<ActDirCopy>(
			getStringViewKey(elem, "source"),
			getStringViewKey(elem, "destination"));
	}

	std::shared_ptr<Action> parseDirCreate(const Value& elem)
	{
		return std::make_shared<ActDirCreate>(getStringViewKey(elem, "file"));
	}

	std::shared_ptr<Action> parseFileCopy(const Value& elem)
	{
		return std::make_shared<ActFileCopy>(
			getStringViewKey(elem, "dir"),
			getStringVectorKey(elem, "file"),
			getStringViewKey(elem, "writeFile"),
			getStringViewKey(elem, "nullText"),
			getReplaceVarsKey(elem, "replace"));
	}

	std::shared_ptr<Action> parseIODelete(const Value& elem)
	{
		return std::make_shared<ActIODelete>(getStringViewKey(elem, "file"));
	}

	std::shared_ptr<Action> parseIODeleteAll(const Value& elem)
	{
		return std::make_shared<ActIODeleteAll>(
			getStringViewKey(elem, "file"),
			getBoolKey(elem, "deleteRoot", true));
	}
}
