#include "Animation.h"
#include "Game.h"
#include "Utils/Utils.h"

Animation::Animation(TexturePackVariant texturePackVar_,
	const std::pair<size_t, size_t>& textureIndexRange, AnimationType type, bool pause)
	: animation(texturePackVar_, textureIndexRange, sf::milliseconds(50), type, pause)
{
	animation.updateTexture(sprite);
}

void Animation::setAnimation(TexturePackVariant texturePackVar_,
	int32_t groupIdx, int32_t directionIdx,
	bool resetAnimation, bool updateAnimationType) noexcept
{
	animation.setTexturePack(std::move(texturePackVar_));
	animation.setAnimation(
		groupIdx, directionIdx, resetAnimation, updateAnimationType
	);
	animation.updateTexture(sprite);
}

void Animation::setAnimation(int32_t groupIdx, int32_t directionIdx,
	bool resetAnimation, bool updateAnimationType) noexcept
{
	animation.setAnimation(
		groupIdx, directionIdx, resetAnimation, updateAnimationType
	);
	animation.updateTexture(sprite);
}

void Animation::update(Game& game)
{
	if (Visible() == true &&
		animation.update(game.getElapsedTime()) == true)
	{
		animation.updateTexture(sprite);
	}
}

bool Animation::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	switch (propHash)
	{
	case str2int16("finished"):
		var = Variable(animation.hasPlayOnceAnimationFinished());
		break;
	case str2int16("isAtBeginning"):
		var = Variable(animation.isAnimationAtBeginning());
		break;
	case str2int16("isAtEnd"):
		var = Variable(animation.isAnimationAtEnd());
		break;
	case str2int16("paused"):
		var = Variable(animation.pause);
		break;
	default:
		return getUIObjProp(propHash, props.second, var);
	}
	return true;
}
