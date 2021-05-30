#pragma once

#include "TexturePack.h"
#include <vector>

class StackedTexturePack : public TexturePack
{
protected:
	std::vector<std::shared_ptr<TexturePack>> texturePacks;
	std::shared_ptr<Palette> palette;

public:
	void addTexturePack(const std::shared_ptr<TexturePack>& texturePack);

	bool get(uint32_t index, TextureInfo& ti) const override;

	int32_t getWidth(uint32_t index) const override;

	void update(int epoch, sf::Time elapsedTime) override;

	const sf::Texture* getTexture() const noexcept override;

	const std::shared_ptr<Palette>& getPalette() const noexcept override;
	uint32_t size() const noexcept override;
};
