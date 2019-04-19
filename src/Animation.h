#pragma once

#include "BaseAnimation.h"
#include <cstddef>
#include "Image.h"
#include <memory>
#include "TexturePacks/TexturePack.h"

class Animation : public Image
{
private:
	std::shared_ptr<TexturePack> texturePack;
	BaseAnimation base;

	void updateTexture();

public:
	Animation(const sf::Texture& texture) : Image(texture) {}

	Animation(const std::shared_ptr<TexturePack>& texturePack_,
		const std::pair<size_t, size_t>& textureIndexRange,
		const sf::Time& frameTime = sf::milliseconds(50),
		AnimationType type = AnimationType::Looped, bool pause = false);

	sf::Time getFrameTime() const noexcept { return base.elapsedTime.timeout; }
	void setFrameTime(sf::Time time) noexcept { base.elapsedTime.timeout = time; }

	virtual void update(Game& game) noexcept;
};
