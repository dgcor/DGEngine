#include "CmdLineUtils.h"
#include "GameUtils.h"
#include "FileUtils.h"
#include "Utils/Utils.h"

namespace CmdLineUtils
{
	bool processCmdLine(int argc, const char* argv[])
	{
		if (argc < 4)
		{
			// no export options
			return false;
		}

		auto mountFiles = Utils::splitString({ argv[2] }, '|');
		size_t numMountedFiles = 0;

		for (const auto& mountFile : mountFiles)
		{
			if (FileUtils::mount(mountFile, "", false) == true)
			{
				numMountedFiles++;
			}
		}
		if (numMountedFiles == 0)
		{
			return true;
		}

		auto commandStr = Utils::splitStringIn2(std::string_view(argv[1]), ':');

		switch (str2int16(commandStr.first))
		{
		case str2int16("--export"):
		{
			if (FileUtils::exists(argv[3]) == true)
			{
				FileUtils::exportFile(argv[3], argv[4]);
			}
			break;
		}
		default:
			break;
		}
		return true;
	}
}
