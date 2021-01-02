#include "Animation.h"
#include "Game.h"
#include "Utils/Utils.h"

Animation::Animation(TexturePackVariant texturePackVar, bool pause)
	: animation(texturePackVar, pause)
{
	animation.updateTexture(sprite);
}

Animation::Animation(TexturePackVariant texturePackVar,
	const AnimationInfo& animInfo, bool pause) : animation(texturePackVar, animInfo, pause)
{
	animation.updateTexture(sprite);
}

void Animation::updateTexture()
{
	animation.updateTexture(sprite);
}

void Animation::setAnimation(const AnimationInfo& animInfo)
{
	animation.setAnimation(animInfo);
	animation.updateTexture(sprite);
}

void Animation::setAnimation(TexturePackVariant texturePackVar,
	const AnimationInfo& animInfo)
{
	animation.setTexturePack(std::move(texturePackVar));
	animation.setAnimation(animInfo);
	animation.updateTexture(sprite);
}

void Animation::setAnimation(int32_t groupIdx, int32_t directionIdx)
{
	animation.setAnimation(groupIdx, directionIdx);
	animation.updateTexture(sprite);
}

void Animation::setAnimation(TexturePackVariant texturePackVar,
	int32_t groupIdx, int32_t directionIdx)
{
	animation.setTexturePack(std::move(texturePackVar));
	animation.setAnimation(groupIdx, directionIdx);
	animation.updateTexture(sprite);
}

void Animation::setAnimation(int32_t groupIdx, int32_t directionIdx,
	bool resetAnimation, bool updateAnimationType)
{
	animation.setAnimation(
		groupIdx, directionIdx, resetAnimation, updateAnimationType
	);
	animation.updateTexture(sprite);
}

void Animation::setAnimation(TexturePackVariant texturePackVar,
	int32_t groupIdx, int32_t directionIdx,
	bool resetAnimation, bool updateAnimationType)
{
	animation.setTexturePack(std::move(texturePackVar));
	animation.setAnimation(
		groupIdx, directionIdx, resetAnimation, updateAnimationType
	);
	animation.updateTexture(sprite);
}

void Animation::setAnimation(int32_t groupIdx, int32_t directionIdx,
	AnimationType animationType)
{
	animation.setAnimation(groupIdx, directionIdx, animationType);
	animation.updateTexture(sprite);
}

void Animation::setAnimation(TexturePackVariant texturePackVar_,
	int32_t groupIdx, int32_t directionIdx, AnimationType animationType)
{
	animation.setTexturePack(std::move(texturePackVar_));
	animation.setAnimation(groupIdx, directionIdx, animationType);
	animation.updateTexture(sprite);
}

bool Animation::holdsNullTexturePack() const noexcept
{
	return animation.holdsNullTexturePack();
}

bool Animation::holdsTexturePack() const noexcept
{
	return animation.holdsTexturePack();
}

bool Animation::holdsTexturePack(const std::shared_ptr<TexturePack>& obj) const noexcept
{
	return animation.holdsTexturePack();
}

bool Animation::holdsCompositeTexture() const noexcept
{
	return animation.holdsCompositeTexture();
}

bool Animation::holdsCompositeTexture(const std::shared_ptr<CompositeTexture>& obj) const noexcept
{
	return animation.holdsCompositeTexture();
}

const std::shared_ptr<TexturePack>& Animation::getTexturePack() const
{
	return animation.getTexturePack();
}

std::shared_ptr<TexturePack>& Animation::getTexturePack()
{
	return animation.getTexturePack();
}

const std::shared_ptr<CompositeTexture>& Animation::getCompositeTexture() const
{
	return animation.getCompositeTexture();
}

std::shared_ptr<CompositeTexture>& Animation::getCompositeTexture()
{
	return animation.getCompositeTexture();
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
