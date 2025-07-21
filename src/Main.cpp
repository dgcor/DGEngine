#include "Game/Game2.h"
#include "Game/Utils/CmdLineUtils2.h"
#include "Game/Utils/FileUtils.h"
#include <iostream>
#include "RegisterHooks.h"
#include "Utils/Log.h"

int main(int argc, char* argv[])
{
	DGENGINE_INIT_LOGGING();
	SPDLOG_INFO("Welcome to DGEngine!");

	Hooks::registerHooks();

	FileUtils::initPhysFS(argv[0]);

	try
	{
		Game2 game;

		if (CmdLineUtils::processCmdLine2(argc, (const char**)argv) == false)
		{
			if (argc == 2)
			{
				game.load(argv[1], "main.json");
			}
			else if (argc == 3)
			{
				game.load(argv[1], argv[2]);
			}
			else
			{
				game.load(".", "main.json");
			}
			game.play();
		}
	}
	catch (std::exception& ex)
	{
#if DGENGINE_HAS_LOGGING
		SPDLOG_ERROR(ex.what());
#else
		std::cerr << ex.what();
#endif
	}

	FileUtils::deinitPhysFS();

	SPDLOG_INFO("Goodbye!");

	return 0;
}
