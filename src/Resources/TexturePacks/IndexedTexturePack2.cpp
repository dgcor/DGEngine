#include "IndexedTexturePack2.h"

bool IndexedTexturePack2::translateIndex(uint32_t& index) const
{
	auto baseTexturePack = dynamic_cast<const TexturePackIndexWrapper*>(getTexturePack());
	if (baseTexturePack != nullptr)
	{
		auto index2 = index;
		baseTexturePack->translateIndex(index2);
		if (index != index2)
		{
			index = index2;
			return true;
		}
	}
	return IndexedTexturePack::translateIndex(index);
}

int32_t IndexedTexturePack2::getFlags(uint32_t index, uint32_t subIndex) const
{
	if (translateIndex(index) == true)
	{
		auto levelFlags = dynamic_cast<const LevelFlags*>(getTexturePack());
		if (levelFlags != nullptr)
		{
			return levelFlags->getFlags(index, subIndex);
		}
	}
	return 0;
}
