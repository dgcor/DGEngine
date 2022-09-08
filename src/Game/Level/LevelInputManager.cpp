#include "LevelInputManager.h"
#include "Game/Game.h"
#include "Game/Player/Player.h"
#include "Level.h"
#include "Utils/Utils.h"

void LevelInputManager::Init(const std::string& eventNamePrefix)
{
	moveUpEventHash = str2int16(eventNamePrefix + ".up");
	moveDownEventHash = str2int16(eventNamePrefix + ".down");
	moveLeftEventHash = str2int16(eventNamePrefix + ".left");
	moveRightEventHash = str2int16(eventNamePrefix + ".right");
	doActionEventHash = str2int16(eventNamePrefix + ".doAction");
}

void LevelInputManager::onMouseButtonPressed(Level& level, Game& game)
{
	game.clearMousePressed();
	switch (game.MousePress().button)
	{
	case sf::Mouse::Left:
	{
		level.clickedMapPosition = level.getMapCoordOverMouse();
		level.levelObjects.clearClickedObject();
		game.Events().tryAddBack(level.leftAction);
	}
	break;
	case sf::Mouse::Right:
		game.Events().tryAddBack(level.rightAction);
		break;
	default:
		break;
	}
}

void LevelInputManager::onMouseScrolled(Level& level, Game& game)
{
	game.clearMouseScrolled();
	if (game.MouseScroll().delta < 0.f)
	{
		game.Events().tryAddBack(level.scrollDownAction);
	}
	else
	{
		game.Events().tryAddBack(level.scrollUpAction);
	}
}

void LevelInputManager::onTouchBegan(Level& level, Game& game)
{
	game.clearTouchBegan();
	switch (game.TouchBegan().finger)
	{
	case 0:
	{
		level.clickedMapPosition = level.getMapCoordOverMouse();
		level.levelObjects.clearClickedObject();
		game.Events().tryAddBack(level.leftAction);
	}
	break;
	case 1:
		game.Events().tryAddBack(level.rightAction);
		break;
	default:
		break;
	}
}

void LevelInputManager::processInputEvents(Level& level, Game& game)
{
	if (game.hasFocus() == false ||
		game.Resources().hasActiveInputEvents() == false)
	{
		return;
	}
	if (auto player = level.levelObjects.CurrentPlayer())
	{
		if (player->blockWalk() == true)
		{
			return;
		}
		PlayerDirection direction(PlayerDirection::All);
		if (game.Resources().hasActiveInputEvents({ moveUpEventHash, moveLeftEventHash }) == true)
		{
			direction = PlayerDirection::BackLeft;
		}
		else if (game.Resources().hasActiveInputEvents({ moveUpEventHash, moveRightEventHash }) == true)
		{
			direction = PlayerDirection::BackRight;
		}
		else if (game.Resources().hasActiveInputEvents({ moveDownEventHash, moveLeftEventHash }) == true)
		{
			direction = PlayerDirection::FrontLeft;
		}
		else if (game.Resources().hasActiveInputEvents({ moveDownEventHash, moveRightEventHash }) == true)
		{
			direction = PlayerDirection::FrontRight;
		}
		else if (game.Resources().hasActiveInputEvents({ moveUpEventHash }) == true)
		{
			direction = PlayerDirection::Back;
		}
		else if (game.Resources().hasActiveInputEvents({ moveDownEventHash }) == true)
		{
			direction = PlayerDirection::Front;
		}
		else if (game.Resources().hasActiveInputEvents({ moveLeftEventHash }) == true)
		{
			direction = PlayerDirection::Left;
		}
		else if (game.Resources().hasActiveInputEvents({ moveRightEventHash }) == true)
		{
			direction = PlayerDirection::Right;
		}
		bool doAction = game.Resources().hasActiveInputEvents({ doActionEventHash });
		player->Walk(level.Map(), direction, doAction);
	}
}

void LevelInputManager::processInput(Level& level, Game& game)
{
	processInputEvents(level, game);

	sf::FloatRect rect(level.surface.Position(), level.surface.Size());
	if (rect.contains(game.MousePositionf()) == true)
	{
		level.hasMouseInside = true;

		if (game.wasMousePressed() == true)
		{
			onMouseButtonPressed(level, game);
		}
		if (game.wasMouseScrolled() == true)
		{
			onMouseScrolled(level, game);
		}
		if (game.hasTouchBegan() == true)
		{
			onTouchBegan(level, game);
		}
		if (level.captureInputEvents != InputEventType::None)
		{
			game.clearInputEvents(level.captureInputEvents);
		}
	}
	else
	{
		level.hasMouseInside = false;
	}
}
