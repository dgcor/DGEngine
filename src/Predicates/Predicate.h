#pragma once

#include "Variable.h"

class Game;

class Predicate
{
public:
	virtual ~Predicate() = default;
	virtual Variable getResult(const Game& game) const = 0;
};
