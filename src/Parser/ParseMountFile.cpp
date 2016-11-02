#include "ParseMountFile.h"
#include "FileUtils.h"
#include <physfs.h>
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseMountFile(Game& game, const Value& elem)
	{
		auto file = getStringKey(elem, "file");
		if (file.empty() == true)
		{
			return;
		}
		file = game.getPath() + file;
		auto mount = getStringCharKey(elem, "mount");
		auto append = getBoolKey(elem, "append");
		if (PHYSFS_mount(file.c_str(), mount, append) != 0)
		{
			return;
		}
		auto fileNoExt = FileUtils::getFileWithoutExt(file);
		if (fileNoExt.length() != file.length())
		{
			if (PHYSFS_mount(fileNoExt.c_str(), mount, append) != 0)
			{
				return;
			}
		}
		auto fileMPQ = fileNoExt + ".mpq";
		if (PHYSFS_mount(fileMPQ.c_str(), mount, append) != 0)
		{
			return;
		}
		auto fileZip = fileNoExt + ".zip";
		if (PHYSFS_mount(fileZip.c_str(), mount, append) != 0)
		{
			return;
		}
		auto file7Zip = fileNoExt + ".7z";
		if (PHYSFS_mount(file7Zip.c_str(), mount, append) != 0)
		{
			return;
		}
	}
}
