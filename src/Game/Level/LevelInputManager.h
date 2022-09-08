#pragma once

#include <string>

class Game;
class Level;

class LevelInputManager
{
private:
	uint16_t moveUpEventHash{ 0 };
	uint16_t moveDownEventHash{ 0 };
	uint16_t moveLeftEventHash{ 0 };
	uint16_t moveRightEventHash{ 0 };
	uint16_t doActionEventHash{ 0 };

	static void onMouseButtonPressed(Level& level, Game& game);
	static void onMouseScrolled(Level& level, Game& game);
	static void onTouchBegan(Level& level, Game& game);

	void processInputEvents(Level& level, Game& game);

public:
	void Init(const std::string& eventNamePrefix);

	void processInput(Level& level, Game& game);
};
