#include "CmdLineUtils.h"
#include "Game.h"
#include "Game/LevelHelper.h"

namespace CmdLineUtils
{
	bool processCmdLine(Game& game, int argc, const char* argv[])
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

		int bottomTopOrBoth = -2;	// invalid

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
		default:
			break;
		}

		if (bottomTopOrBoth != -2 &&
			FileUtils::exists(argv[3]) == true)
		{
			if (argc > 5)
			{
				if (FileUtils::exists(argv[4]) == true)
				{
					LevelHelper::loadAndSaveTilesetSprite(
						argv[3], argv[4], argv[5], bottomTopOrBoth, false, 5120, minBlocks
					);
				}
			}
			else
			{
				LevelHelper::loadAndSaveTilesetSprite(
					argv[3], "", argv[4], bottomTopOrBoth, false, 5120, minBlocks
				);
			}
		}
		return true;
	}
}
