#include "Parser.h"
#include "Game.h"
#include "FileUtils.h"
#include "ParseFile.h"

namespace Parser
{
	using namespace rapidjson;

	void parseGame(Game& game, const std::string_view filePath, const std::string_view fileName)
	{
		if (FileUtils::mount(filePath, "", false) == false)
		{
			throw std::runtime_error(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		}

		rapidjson::Document doc;  // Default template parameter uses UTF8 and MemoryPoolAllocator.
		auto jsonText = FileUtils::readText(fileName.data());
		if (doc.Parse(jsonText.data(), jsonText.size()).HasParseError())
		{
			FileUtils::unmount(filePath);
			return;
		}

		auto filePath2 = FileUtils::getFilePath(filePath);
		if (filePath2.empty() == false)
		{
			filePath2 += PHYSFS_getDirSeparator();
		}
		else
		{
			filePath2 = std::string(".") + PHYSFS_getDirSeparator();
		}

		game.setPath(filePath2);

		parseDocument(game, doc);
	}
}
