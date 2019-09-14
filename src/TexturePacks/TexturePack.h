#pragma once

#include "AnimationType.h"
#include "Game/LevelFlags.h"
#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>
#include <utility>

class Palette;
struct TextureInfo;

class TexturePack : public LevelFlags
{
protected:
	static void updateTextureRect(TextureInfo& ti);

	static std::pair<uint32_t, uint32_t> getRange(uint32_t startIdx,
		uint32_t stopIdx, int32_t directionIdx, uint32_t directions);

	TexturePack(TexturePack const&) = delete;
	TexturePack& operator=(TexturePack const&) = delete;

public:
	TexturePack() = default;
	virtual ~TexturePack() = default;

	// if texture is an index texture, sets the palette in TextureInfo
	virtual bool get(uint32_t index, TextureInfo& ti) const = 0;

	virtual void update(int epoch, sf::Time elapsedTime) {}

	// returns a texture only if the same texture is used for all calls to get
	// returns nullptr if more than one texture is used
	virtual const sf::Texture* getTexture() const noexcept { return nullptr; }

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept = 0;
	virtual uint32_t size() const noexcept = 0;

	virtual uint32_t getGroupCount() const noexcept { return 1; }
	virtual uint32_t getDirectionCount(uint32_t groupIdx) const noexcept { return 1; }
	virtual uint32_t getDirection(uint32_t frameIdx) const noexcept { return 0; }

	virtual std::pair<uint32_t, uint32_t> getRange(
		int32_t groupIdx, int32_t directionIdx, AnimationType& animType) const;

	std::pair<uint32_t, uint32_t> getRange(int32_t groupIdx, int32_t directionIdx) const
	{
		AnimationType animType;
		return getRange(groupIdx, directionIdx, animType);
	}
};
