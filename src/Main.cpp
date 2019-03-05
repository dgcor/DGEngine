#include <iostream>
#include "Game.h"

int main(int argc, char* argv[])
{
	FileUtils::initPhysFS(argv[0]);

	try
	{
		Game game;

#ifdef __ANDROID__
		game.load("/sdcard/gamefiles.zip", "main.json");
#else
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
#endif
		game.play();
	}
	catch (std::exception ex)
	{
		std::cerr << ex.what();
	}

	FileUtils::deinitPhysFS();
	return 0;
}
