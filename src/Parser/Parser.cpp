#include "Parser.h"
#include "FileUtils.h"
#include "ParseFile.h"
#include "ParseUtils.h"
#include "Utils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseGame(Game& game, std::string filePath, const std::string& fileName)
	{
		if (PHYSFS_mount(filePath.c_str(), NULL, 0) == 0)
		{
			throw std::runtime_error(PHYSFS_getLastError());
		}

		rapidjson::Document doc;  // Default template parameter uses UTF8 and MemoryPoolAllocator.

		if (doc.Parse(FileUtils::readText(fileName.c_str()).c_str()).HasParseError())
		{
#if (PHYSFS_VER_MAJOR >= 2 && PHYSFS_VER_MINOR >= 1)
			PHYSFS_unmount(filePath.c_str());
#else
			PHYSFS_removeFromSearchPath(filePath.c_str());
#endif
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
