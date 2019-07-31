#include "CmdLineUtils.h"
#include "GameUtils.h"
#ifndef NO_DIABLO_FORMAT_SUPPORT
#include "Game/LevelHelper.h"
#endif
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

		unsigned minBlocks = 10;
		auto commandStr = Utils::splitStringIn2(std::string_view(argv[1]), ':');
		if (commandStr.second.empty() == false)
		{
			minBlocks = Utils::strtou(commandStr.second);
			if (minBlocks < 10)
			{
				minBlocks = 10;
			}
		}

#ifndef NO_DIABLO_FORMAT_SUPPORT
		int bottomTopOrBoth = -2;	// invalid
#endif

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
#ifndef NO_DIABLO_FORMAT_SUPPORT
		case str2int16("--export-tileset-bottom"):
		case str2int16("--export-tileset-back"):
			bottomTopOrBoth = 0;
			break;
		case str2int16("--export-tileset-top"):
		case str2int16("--export-tileset-front"):
			bottomTopOrBoth = 1;
			break;
		case str2int16("--export-tileset"):
			bottomTopOrBoth = -1;
			break;
#endif
		default:
			break;
		}

#ifndef NO_DIABLO_FORMAT_SUPPORT
		if (bottomTopOrBoth != -2 &&
			FileUtils::exists(argv[3]) == true)
		{
			if (argc > 5)
			{
				auto palStr = Utils::splitStringIn2(std::string_view(argv[4]), ':');
				auto palPath = std::string(palStr.first);
				if (FileUtils::exists(palPath.c_str()) == true)
				{
					auto colorFormat = GameUtils::getColorFormat(palStr.second);

					LevelHelper::loadAndSaveTilesetSprite(
						argv[3], palPath, colorFormat, argv[5],
						bottomTopOrBoth, false, 5120, minBlocks
					);
				}
			}
			else
			{
				LevelHelper::loadAndSaveTilesetSprite(
					argv[3], "", Palette::ColorFormat::RGB, argv[4],
					bottomTopOrBoth, false, 5120, minBlocks
				);
			}
		}
#endif
		return true;
	}
}
