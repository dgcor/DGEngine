# DGEngine core

DGEngine core is a 2D game engine.

### Example

Here is a simple example on how to use DGEngine core:

```cpp
#include "CmdLineUtils.h"
#include "FileUtils.h"
#include "Game.h"
#include <iostream>

int main(int argc, char* argv[])
{
	FileUtils::initPhysFS(argv[0]);

	try
	{
		Game game;

		if (CmdLineUtils::processCmdLine(argc, (const char**)argv) == false)
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
```
