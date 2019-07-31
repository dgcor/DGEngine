#include "LoadingScreen.h"
#include <cmath>
#include "Utils/Utils.h"

std::shared_ptr<Action> LoadingScreen::getAction(uint16_t nameHash16) const noexcept
{
	switch (nameHash16)
	{
	case str2int16("complete"):
		return completeAction;
	default:
		return nullptr;
	}
}

bool LoadingScreen::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept
{
	switch (nameHash16)
	{
	case str2int16("complete"):
		completeAction = action;
		break;
	default:
		return false;
	}
	return true;
}

void LoadingScreen::Position(const sf::Vector2f& position_)
{
	Animation::Position(position_);
	progressBar.setPosition(position_ + barPosOffset);
}

void LoadingScreen::updateSize(const Game& game)
{
	Animation::updateSize(game);
	progressBar.setPosition(Animation::Position() + barPosOffset);
}

void LoadingScreen::setProgress(int percent_)
{
	percent = std::clamp(percent_, 0, 100);

	sf::Vector2f newSize(barSize);
	newSize.x = std::round(newSize.x * ((float)percent / 100.f));
	progressBar.setSize(newSize);

	if (animation.holdsNullTexturePack() == false)
	{
		auto range = animation.textureIndexRange.second -
			animation.textureIndexRange.first;
		animation.currentTextureIdx = (size_t)std::round(
			(float)animation.textureIndexRange.first +
			(float)range * ((float)percent / 100.f)
		);
		animation.updateTexture(sprite);
	}
}

void LoadingScreen::draw(const Game& game, sf::RenderTarget& target) const
{
	Animation::draw(game, target);
	target.draw(progressBar);
}
