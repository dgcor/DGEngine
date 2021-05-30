#include "CmdLineUtils2.h"
#include "FileUtils.h"
#include "Game2.h"
#include <iostream>
#include "RegisterHooks.h"

int main(int argc, char* argv[])
{
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
		std::cerr << ex.what();
	}

	FileUtils::deinitPhysFS();
	return 0;
}
