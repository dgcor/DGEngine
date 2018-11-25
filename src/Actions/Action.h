#pragma once

class Game;

class Action
{
public:
	virtual ~Action() = default;
	virtual bool execute(Game& game) = 0;
};
