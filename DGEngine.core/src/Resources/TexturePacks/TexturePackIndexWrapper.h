#pragma once

#include "TexturePackWrapper.h"

class TexturePackIndexWrapper : public TexturePackWrapper
{
public:
	TexturePackIndexWrapper(std::unique_ptr<TexturePack> texturePack_)
		: TexturePackWrapper(std::move(texturePack_)) {}

	// returns true if index exists and false otherwise
	virtual bool translateIndex(uint32_t& index) const = 0;

	// only works if translateIndex returns true
	bool get(uint32_t index, TextureInfo& ti) const override;

	// only works if translateIndex returns true
	sf::Vector2i getTextureSize(uint32_t index) const override;
};
