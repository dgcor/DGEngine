#pragma once

#include <memory>
#include "Palette.h"
#include "TextureInfo.h"

class TexturePack
{
protected:
	static void updateTextureInfo(TextureInfo& ti);

public:
	virtual ~TexturePack() = default;

	// if texture is an index texture, sets the palette in TextureInfo
	virtual bool get(size_t index, TextureInfo& ti) const = 0;

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept = 0;
	virtual bool isIndexed() const noexcept = 0;
	virtual size_t size() const noexcept = 0;
};
