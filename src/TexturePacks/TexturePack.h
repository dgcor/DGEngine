#pragma once

#include <memory>
#include <SFML/System/Time.hpp>

class Palette;
struct TextureInfo;

class TexturePack
{
protected:
	static void updateTextureRect(TextureInfo& ti);

	TexturePack(TexturePack const &) = delete;
	TexturePack& operator=(TexturePack const &) = delete;

public:
	TexturePack() = default;
	virtual ~TexturePack() = default;

	// if texture is an index texture, sets the palette in TextureInfo
	virtual bool get(size_t index, TextureInfo& ti) const = 0;

	virtual void update(sf::Time elapsedTime) {};

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept = 0;
	virtual bool isIndexed() const noexcept = 0;
	virtual size_t size() const noexcept = 0;
};
