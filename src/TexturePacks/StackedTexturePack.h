#pragma once

#include "TexturePack.h"
#include <vector>

class StackedTexturePack : public TexturePack
{
private:
	std::vector<std::shared_ptr<TexturePack>> texturePacks;
	std::shared_ptr<Palette> palette;

public:
	void addTexturePack(const std::shared_ptr<TexturePack>& texturePack);

	virtual bool get(uint32_t index, TextureInfo& ti) const;

	virtual void update(int epoch, sf::Time elapsedTime);

	virtual const sf::Texture* getTexture() const noexcept;

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept;
	virtual uint32_t size() const noexcept;
};
