#pragma once

#include "PlayerBase.h"
#include "Save/SavePlayer.h"

class Player : public PlayerBase
{
private:
	void updateAI(Level& level);
	void updateAnimation(const Game& game);
	void updateWalk(Game& game, Level& level);
	void updateAttack(Game& game, Level& level);
	void updateDead(Game& game, Level& level);

	friend void Save::serialize(void* serializeObj, const Save::Properties& props,
		const Game& game, const Level& level, const Player& player);

public:
	using PlayerBase::PlayerBase;

	void serialize(void* serializeObj, Save::Properties& props,
		const Game& game, const Level& level) const override;

	void update(Game& game, Level& level, std::weak_ptr<LevelObject> thisPtr) override;
};
