#pragma once

#include "TexturePacks/CompositeTexturePack.h"

class CompositeTexturePack2 : public CompositeTexturePack
{
public:
#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
	using CompositeTexturePack::addGroup;

	void addGroup(const std::string_view fileName, bool fixLayerOrdering);
#endif
};
