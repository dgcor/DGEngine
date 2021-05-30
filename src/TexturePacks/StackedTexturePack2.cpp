#include "StackedTexturePack2.h"

int32_t StackedTexturePack2::getFlags(uint32_t index, uint32_t subIndex) const
{
	for (const auto& texturePack : texturePacks)
	{
		auto levelFlags = dynamic_cast<const LevelFlags*>(texturePack.get());
		if (levelFlags != nullptr)
		{
			if (auto flags = levelFlags->getFlags(index, subIndex))
			{
				return flags;
			}
		}
	}
	return 0;
}
