#include "ParseFileBytes.h"
#include "FileUtils.h"
#include "Game.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	bool parseFileBytesFromId(Game& game, const Value& elem)
	{
		if (isValidString(elem, "fromId") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				auto fromId = elem["fromId"sv].GetStringView();
				auto id = elem["id"sv].GetStringView();
				if (fromId != id && isValidId(id) == true)
				{
					auto obj = game.Resources().getFileBytes(fromId);
					if (obj != nullptr)
					{
						game.Resources().addFileBytes(id, obj, getStringViewKey(elem, "resource"));
					}
				}
			}
			return true;
		}
		return false;
	}

	void parseFileBytes(Game& game, const Value& elem)
	{
		if (parseFileBytesFromId(game, elem) == true)
		{
			return;
		}

		if (isValidString(elem, "id") == false)
		{
			return;
		}
		auto id = elem["id"sv].GetStringView();
		if (isValidId(id) == false)
		{
			return;
		}

		if (elem.HasMember("file"sv) == false)
		{
			return;
		}

		auto bytes = FileUtils::readChar(getStringCharVal(elem["file"sv]));
		if (bytes.empty() == true)
		{
			return;
		}
		auto fileBytes = std::make_shared<FileBytes>(std::move(bytes));

		game.Resources().addFileBytes(id, fileBytes, getStringViewKey(elem, "resource"));
	}
}
