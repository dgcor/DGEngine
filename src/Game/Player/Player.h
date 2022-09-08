#pragma once

#include "PlayerBase.h"

class Player : public PlayerBase
{
private:
	void updateAI(Level& level);
	void updateAnimation(const Game& game);
	void updateWalk(Game& game, Level& level);
	void updateAttack(Game& game, Level& level);
	void updateDead(Game& game, Level& level);

public:
	using PlayerBase::PlayerBase;

	void update(Game& game, Level& level, const std::shared_ptr<LevelObject>& thisPtr) override;
};
