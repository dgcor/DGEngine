#pragma once

#include "Game/AnimationType.h"
#include "Resources/TexturePack.h"
#include <vector>

struct TexturePackGroup
{
	std::shared_ptr<sf::Texture> texture;
	sf::Vector2f offset;
	uint32_t startIndex{ 0 };
	uint32_t maxFrames{ 0 };
	uint32_t numFrames{ 0 };
	uint32_t subImageSizeX{ 0 };
	uint32_t subImageSizeY{ 0 };
	uint32_t directions{ 0 };
	std::vector<std::pair<uint32_t, uint32_t>> directionsVec;
	AnimationType animType{ AnimationType::Looped };
	sf::Time refresh;
	bool horizontalDirection{ false };

	bool makeTexturePack(const std::pair<uint32_t, uint32_t>& frames);

	void getTexture(uint32_t index, TextureInfo& ti) const noexcept;

	uint32_t getDirection(uint32_t frameIdx) const noexcept;
};
