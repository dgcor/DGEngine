#include "Animation.h"
#include "Game.h"

Animation::Animation(const std::shared_ptr<TexturePack>& texturePack_,
	const std::pair<size_t, size_t>& textureIndexRange, const sf::Time& frameTime,
	AnimationType type, bool pause) : texturePack(texturePack_),
	base(textureIndexRange, frameTime, type, pause)
{
	if (texturePack_->isIndexed() == true)
	{
		setPalette(texturePack_->getPalette());
	}
	updateTexture();
}

void Animation::updateTexture()
{
	TextureInfo ti;
	if (texturePack->get(base.currentTextureIdx, ti) == true)
	{
		setTexture(*ti.texture);
		setTextureRect(ti.textureRect);
	}
}

void Animation::update(Game& game) noexcept
{
	if (Visible() == true &&
		base.update(game.getElapsedTime()) == true)
	{
		updateTexture();
	}
}
