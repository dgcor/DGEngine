#include "ParseMountFile.h"
#include "FileUtils.h"
#include "ParseUtils.h"
#include <physfs.h>

namespace Parser
{
	using namespace rapidjson;

	void parseMountFile(Game& game, const Value& elem)
	{
		auto file = getString(elem, "file");
		if (file.empty() == true)
		{
			return;
		}
		file = game.getPath() + file;
		auto mount = getStringChar(elem, "mount");
		auto append = getBool(elem, "append");
		auto success = PHYSFS_mount(file.c_str(), mount, append);
		if (success == 0)
		{
			auto file2 = FileUtils::getFileWithoutExt(file);
			if (file2.length() != file.length())
			{
				PHYSFS_mount(file2.c_str(), mount, append);
			}
		}
	}
}
