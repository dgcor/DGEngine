#include "ParseFileBytes.h"
#include "Game/Game.h"
#include "Game/Utils/FileUtils.h"
#include "Parser/ParseCommon.h"
#include "Parser/Utils/ParseUtils.h"
#include "ParseResource.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseFileBytes(Game& game, const Value& elem)
	{
		if (parseResourceFromId<std::shared_ptr<FileBytes>>(game.Resources(), elem) == true)
		{
			return;
		}

		if (elem.HasMember("file"sv) == false)
		{
			return;
		}

		auto file = getStringViewVal(elem["file"sv]);
		auto id = parseValidIdOrFilePath(elem, file);

		if (id.empty() == true)
		{
			return;
		}

		auto bytes = FileUtils::readChar(file);
		if (bytes.empty() == true)
		{
			return;
		}
		auto fileBytes = std::make_shared<FileBytes>(std::move(bytes));

		game.Resources().addFileBytes(id, fileBytes, getStringViewKey(elem, "resource"));
	}
}
