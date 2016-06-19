#pragma once

class Game;

class Action
{
public:
	virtual ~Action() {}
	virtual bool execute(Game& game) { return true; }
};
