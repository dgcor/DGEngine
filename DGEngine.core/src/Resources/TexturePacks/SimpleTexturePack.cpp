#include "SimpleTexturePack.h"

SimpleTexturePack::SimpleTexturePack(TexturePackGroup&& t_,
	const std::shared_ptr<Palette>& palette_) : SingleTexturePack(std::move(t_), palette_)
{
	if (t.texture == nullptr)
	{
		return;
	}
	t.numFrames = 1;
	t.maxFrames = 1;
	auto imgSize = t.texture->getSize();
	t.subImageSizeX = imgSize.x;
	t.subImageSizeY = imgSize.y;
	if (t.directionsVec.empty() == false)
	{
		t.directions = (uint32_t)t.directionsVec.size();
	}
	else
	{
		t.directions = std::max(t.directions, 1u);
	}
	palette = palette_;
}

bool SimpleTexturePack::get(uint32_t index, TextureInfo& ti) const noexcept
{
	if (index > 0)
	{
		return false;
	}
	ti.texture = t.texture.get();
	ti.textureRect.top = 0;
	ti.textureRect.left = 0;
	ti.textureRect.width = t.subImageSizeX;
	ti.textureRect.height = t.subImageSizeY;
	ti.palette = palette;
	ti.offset = t.offset;
	ti.absoluteOffset = false;
	ti.blendMode = BlendMode::Alpha;
	ti.nextIndex = -1;
	return true;
}
