#pragma once

#include "Game/AnimationFlags.h"
#include "Game/TextureInfo.h"
#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>
#include <utility>
#include <vector>

struct AnimationInfo;

class TexturePack
{
protected:
	static void updateTextureRect(TextureInfo& ti);

public:
	TexturePack() = default;
	virtual ~TexturePack() = default;

	TexturePack(TexturePack const&) = delete;
	TexturePack& operator=(TexturePack const&) = delete;

	// if texture is an index texture, sets the palette in TextureInfo
	virtual bool get(uint32_t index, TextureInfo& ti) const = 0;

	virtual bool get(uint32_t index, TextureInfoVar& tiVar) const;

	virtual sf::Vector2i getTextureSize(uint32_t index) const = 0;

	virtual void update(int epoch, sf::Time elapsedTime) {}

	// returns a texture only if the same texture is used for all calls to get
	// returns nullptr if more than one texture is used
	virtual const sf::Texture* getTexture() const noexcept { return nullptr; }

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept = 0;

	virtual uint32_t size() const noexcept = 0;

	virtual uint32_t getGroupCount() const noexcept { return 1; }

	virtual uint32_t getDirectionCount(uint32_t groupIdx) const noexcept { return 1; }

	// returns a pair with group and direction
	virtual std::pair<uint32_t, uint32_t> getDirection(uint32_t frameIdx, AnimationFlags& flags) const noexcept;

	uint32_t getDirection(uint32_t frameIdx) const noexcept;

	virtual AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const;
};
