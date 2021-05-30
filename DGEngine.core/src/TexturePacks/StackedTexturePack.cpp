#include "StackedTexturePack.h"

void StackedTexturePack::addTexturePack(const std::shared_ptr<TexturePack>& texturePack)
{
	texturePacks.push_back(texturePack);
}

bool StackedTexturePack::get(uint32_t index, TextureInfo& ti) const
{
	for (const auto& texturePack : texturePacks)
	{
		if (texturePack->get(index, ti) == true)
		{
			return true;
		}
	}
	return false;
}

int32_t StackedTexturePack::getWidth(uint32_t index) const
{
	for (const auto& texturePack : texturePacks)
	{
		auto width = texturePack->getWidth(index);
		if (width > 0)
		{
			return width;
		}
	}
	return 0;
}

void StackedTexturePack::update(int epoch, sf::Time elapsedTime)
{
	return;
	for (auto& texturePack : texturePacks)
	{
		texturePack->update(epoch, elapsedTime);
	}
}

const sf::Texture* StackedTexturePack::getTexture() const noexcept
{
	if (texturePacks.size() == 1)
	{
		return texturePacks.front()->getTexture();
	}
	return nullptr;
}

const std::shared_ptr<Palette>& StackedTexturePack::getPalette() const noexcept
{
	if (texturePacks.empty() == false)
	{
		return texturePacks.front()->getPalette();
	}
	return palette;
}

uint32_t StackedTexturePack::size() const noexcept
{
	if (texturePacks.empty() == false)
	{
		return texturePacks.front()->size();
	}
	return 0;
}
