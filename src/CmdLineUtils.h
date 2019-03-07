#pragma once

class Game;

namespace CmdLineUtils
{
	// returns true if any export command was found (reagrdless of success)
	bool processCmdLine(Game& game, int argc, const char* argv[]);
}
