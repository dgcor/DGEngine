#include "Parser.h"
#include "FileUtils.h"
#include "ParseFile.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseGame(Game& game, std::string filePath, const std::string_view fileName)
	{
		if (PHYSFS_mount(filePath.c_str(), nullptr, 0) == 0)
		{
			throw std::runtime_error(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		}

		rapidjson::Document doc;  // Default template parameter uses UTF8 and MemoryPoolAllocator.
		auto jsonText = FileUtils::readText(fileName.data());
		if (doc.Parse(jsonText.data(), jsonText.size()).HasParseError())
		{
			PHYSFS_unmount(filePath.c_str());
			return;
		}

		filePath = Utils::trimEnd(filePath, "/\\");
		auto lastIdx = filePath.find_last_of(PHYSFS_getDirSeparator());
		if (lastIdx != std::string::npos)
		{
			filePath = filePath.substr(0, lastIdx);

			if (Utils::endsWith(filePath, "\\") == false &&
				Utils::endsWith(filePath, "/") == false)
			{
				filePath += PHYSFS_getDirSeparator();
			}
		}
		else
		{
			filePath = std::string(".") + PHYSFS_getDirSeparator();
		}

		game.setPath(filePath);

		parseDocument(game, doc);
	}
}
