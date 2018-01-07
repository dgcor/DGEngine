#include "Animation.h"
#include "Game.h"

Animation::Animation(const std::shared_ptr<TexturePack>& texturePack_,
	size_t frameStart_, size_t frameEnd_, const sf::Time& frameTime,
	bool pause_, bool loop_) : texturePack(texturePack_),
	frameStart(frameStart_), frameEnd(frameEnd_), frameIdx(frameStart_),
	frameTime(frameTime), pause(pause_), loop(loop_)
{
	if (texturePack_->isIndexed() == true)
	{
		setPalette(texturePack_->getPalette());
	}
	updateTexture();
}

void Animation::updateTexture()
{
	const sf::Texture* texture;
	sf::IntRect rect;
	if (texturePack->get(frameIdx, &texture, rect) == true)
	{
		setTexture(*texture);
		setTextureRect(rect);
	}
}

void Animation::updateFrameIndex()
{
	if (frameIdx < frameEnd)
	{
		frameIdx++;
	}
	else
	{
		frameIdx = frameStart;

		if (loop == false)
		{
			pause = true;
		}
	}
}

void Animation::update(Game& game)
{
	if (pause == true ||
		frameEnd <= frameStart ||
		Visible() == false)
	{
		return;
	}

	// add delta time
	currentTime += game.getElapsedTime();

	// if current time is bigger then the frame time advance one frame
	if (currentTime >= frameTime)
	{
		// reset time, but keep the remainder
		currentTime = sf::microseconds(currentTime.asMicroseconds() % frameTime.asMicroseconds());

		updateTexture();
		updateFrameIndex();
	}
}
