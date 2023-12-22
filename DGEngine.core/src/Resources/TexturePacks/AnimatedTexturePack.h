#pragma once

#include "TexturePackIndexWrapper.h"
#include <unordered_map>
#include "Utils/ElapsedTime.h"
#include <vector>

class AnimatedTexturePack : public TexturePackIndexWrapper
{
protected:
	struct TexturePackAnimation
	{
		ElapsedTime refresh;
		uint32_t currentIndex{ 0 };
		std::vector<uint32_t> indexes;

		uint32_t getCurrentAnimationIndex() const noexcept { return indexes[currentIndex]; }
	};

	std::vector<TexturePackAnimation> animatedTextures;
	std::unordered_map<uint32_t, uint32_t> animatedIndexes;
	int lastEpoch{ 0 };

public:
	AnimatedTexturePack(std::unique_ptr<TexturePack> texturePack_);

	void addAnimatedTexture(uint32_t animIndex, sf::Time refresh, const std::vector<uint32_t>& indexes);

	bool translateIndex(uint32_t& index) const override;

	void update(int epoch, sf::Time elapsedTime) override;
};
