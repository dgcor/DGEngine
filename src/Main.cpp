#include <iostream>
#include "Parser/Parser.h"

int main(int argc, char *argv[])
{
	PHYSFS_init(NULL);
	PHYSFS_permitSymbolicLinks(1);

	try
	{
		Game game;

#ifdef __ANDROID__
		Parser::parseGame(game, "/sdcard/gamefiles.zip", "main.json");
#else
		if (argc == 2)
		{
			Parser::parseGame(game, argv[1], "main.json");
		}
		else if (argc == 3)
		{
			Parser::parseGame(game, argv[1], argv[2]);
		}
		else
		{
			Parser::parseGame(game, ".", "main.json");
		}
#endif
		game.play();
	}
	catch (std::exception ex)
	{
		std::cerr << ex.what();
	}

	PHYSFS_deinit();
	return 0;
}
