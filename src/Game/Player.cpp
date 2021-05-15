#include "Player.h"
#include "Game.h"
#include "Level.h"

void Player::serialize(void* serializeObj, Save::Properties& props,
	const Game& game, const Level& level) const
{
	Save::serialize(serializeObj, props, game, level, *this);
}

void Player::updateAI(Level& level)
{
	switch (playerStatus)
	{
	case PlayerStatus::Walk:
		return;
	default:
		break;
	}
	auto plr = level.getCurrentPlayer();
	if (plr != nullptr)
	{
		setWalkPath(level.Map().getPath(mapPosition, plr->MapPosition()), false);
	}
}

void Player::updateAnimation(const Game& game)
{
	if (animation.update(game.getElapsedTime()))
	{
		updateTexture();
	}
}

void Player::updateWalk(Game& game, Level& level)
{
	updateWalkPath(game, level.Map());
	updateAnimation(game);
}

void Player::updateAttack(Game& game, Level& level)
{
	updateAnimation(game);
}

void Player::updateDead(Game& game, Level& level)
{
	if (playerAnimation != PlayerAnimation::Die1)
	{
		animation.currentTextureIdx = 0;
		setAnimation(PlayerAnimation::Die1);
	}
	if (animation.currentTextureIdx >= animation.textureIndexRange.second)
	{
		return;
	}
	updateAnimation(game);
}

void Player::update(Game& game, Level& level, std::weak_ptr<LevelObject> thisPtr)
{
	processQueuedActions(game);

	if (hasValidState() == false)
	{
		return;
	}
	if (aiType != 0)
	{
		updateAI(level);
	}

	if (playerStatus != PlayerStatus::Dead &&
		LifeNow() <= 0)
	{
		playerStatus = PlayerStatus::Dead;
		playSound("die");
	}

	switch (playerStatus)
	{
	default:
	case PlayerStatus::Stand:
		updateAnimation(game);
		break;
	case PlayerStatus::Walk:
		updateWalk(game, level);
		break;
	case PlayerStatus::Attack:
		updateAttack(game, level);
		break;
	case PlayerStatus::Dead:
		updateDead(game, level);
		break;
	}

	updateHover(game, level, thisPtr);
}
