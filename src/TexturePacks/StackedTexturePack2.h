#pragma once

#include "Game/LevelFlags.h"
#include "TexturePacks/StackedTexturePack.h"

class StackedTexturePack2 : public StackedTexturePack, public LevelFlags
{
public:
	using StackedTexturePack::StackedTexturePack;

	int32_t getFlags(uint32_t index, uint32_t subIndex) const override;
};
