#include "AnimatedTexturePack.h"
#include "Game/AnimationInfo.h"

AnimatedTexturePack::AnimatedTexturePack(std::unique_ptr<TexturePack> texturePack_)
	: TexturePackIndexWrapper(std::move(texturePack_)) {}

void AnimatedTexturePack::addAnimatedTexture(uint32_t animIndex, sf::Time refresh, const std::vector<uint32_t>& indexes)
{
	auto it = animatedIndexes.find(animIndex);
	if (it != animatedIndexes.cend())
	{
		return;
	}
	animatedIndexes[animIndex] = (uint32_t)animatedTextures.size();
	TexturePackAnimation anim;
	anim.refresh = refresh;
	anim.indexes = indexes;
	animatedTextures.push_back(anim);
}

bool AnimatedTexturePack::translateIndex(uint32_t& index) const
{
	auto it = animatedIndexes.find(index);
	if (it != animatedIndexes.cend())
	{
		index = animatedTextures[it->second].getCurrentAnimationIndex();
	}
	return true;
}

void AnimatedTexturePack::update(int epoch, sf::Time elapsedTime)
{
	if (lastEpoch == epoch)
	{
		return;
	}
	lastEpoch = epoch;

	for (auto& anim : animatedTextures)
	{
		anim.refresh.update(elapsedTime, [&anim]() -> void
		{
			if (anim.currentIndex + 1 < anim.indexes.size())
			{
				anim.currentIndex++;
			}
			else
			{
				anim.currentIndex = 0;
			}
		});
	}
}
