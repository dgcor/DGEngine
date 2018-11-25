#include "ParseMountFile.h"
#include "FileUtils.h"
#include <physfs.h>
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseMountFile(const Game& game, const Value& elem)
	{
		auto file = getStringKey(elem, "file");
		if (file.empty() == true)
		{
			return;
		}

		file = game.getPath() + file;

		if (getBoolKey(elem, "prependSaveDir") == true)
		{
			std::string saveDir = FileUtils::getSaveDir();

			if (Utils::endsWith(saveDir, "\\") == false &&
				Utils::endsWith(saveDir, "/") == false)
			{
				saveDir += PHYSFS_getDirSeparator();
			}
			file = saveDir + file;
		}
		if (getBoolKey(elem, "unmount") == false)
		{
			auto mount = getStringCharKey(elem, "mount");
			int append = (int)getBoolKey(elem, "append", true);

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
		else
		{
			if (PHYSFS_unmount(file.c_str()) != 0)
			{
				return;
			}
			auto fileNoExt = FileUtils::getFileWithoutExt(file);
			if (fileNoExt.length() != file.length())
			{
				if (PHYSFS_unmount(fileNoExt.c_str()) != 0)
				{
					return;
				}
			}
			auto fileMPQ = fileNoExt + ".mpq";
			if (PHYSFS_unmount(fileMPQ.c_str()) != 0)
			{
				return;
			}
			auto fileZip = fileNoExt + ".zip";
			if (PHYSFS_unmount(fileZip.c_str()) != 0)
			{
				return;
			}
			auto file7Zip = fileNoExt + ".7z";
			if (PHYSFS_unmount(file7Zip.c_str()) != 0)
			{
				return;
			}
		}
	}
}
