#pragma once

#include <cstdint>

struct TextureInfo;

class TextureQueryable
{
public:
	virtual bool getTexture(uint32_t textureNumber, TextureInfo& ti) const { return false; }
};
