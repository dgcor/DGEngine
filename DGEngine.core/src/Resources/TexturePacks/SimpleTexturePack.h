#pragma once

#include "SingleTexturePack.h"

// TexturePack that only has 1 texture for index 0
class SimpleTexturePack : public SingleTexturePack
{
public:
	SimpleTexturePack(TexturePackGroup&& t_, const std::shared_ptr<Palette>& palette_);

	void setSize(uint32_t size_) noexcept { t.numFrames = std::max(size_, 1u); }

	bool get(uint32_t index, TextureInfo& ti) const noexcept override;
};
