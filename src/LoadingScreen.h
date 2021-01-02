#pragma once

#include "Animation.h"
#include <SFML/Graphics/RectangleShape.hpp>

class LoadingScreen : public Animation
{
private:
	sf::RectangleShape progressBar;
	sf::Vector2f barSize;
	sf::Vector2f barPosOffset;
	std::shared_ptr<Action> completeAction;
	int percent{ 0 };

public:
	LoadingScreen() {}

	LoadingScreen(const sf::Texture& texture) : Animation(texture) {}

	LoadingScreen(const std::shared_ptr<TexturePack>& texturePack_,
		const AnimationInfo& animInfo) : Animation(texturePack_, animInfo) {}

	std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept override;
	bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept override;

	void Position(const sf::Vector2f& position_) override;

	void setProgressBarColor(const sf::Color& color) { progressBar.setFillColor(color); }
	void setProgressBarPositionOffset(const sf::Vector2f& offset)
	{
		barPosOffset = offset;
		progressBar.setPosition(sprite.getPosition() + offset);
	}
	void setProgressBarSize(const sf::Vector2f& size) noexcept { barSize = size; }
	int getProgress() const noexcept { return percent; }
	void setProgress(int percent_);
	bool isComplete() const noexcept { return percent >= 100; }

	void updateSize(const Game& game) override;

	void draw(const Game& game, sf::RenderTarget& target) const override;
};
