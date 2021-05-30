#include "IndexedTexturePack2.h"

int32_t IndexedTexturePack2::getFlags(uint32_t index, uint32_t subIndex) const
{
	if (translateIndex(index) == true)
	{
		auto levelFlags = dynamic_cast<const LevelFlags*>(texturePack.get());
		if (levelFlags != nullptr)
		{
			return levelFlags->getFlags(index, subIndex);
		}
	}
	return 0;
}
