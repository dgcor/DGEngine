#pragma once

#include "Action.h"
#include "FadeInOut.h"
#include "Game.h"
#include <memory>
#include <string>

class ActGameFade : public Action
{
private:
	sf::Color color;
	bool isFadeOut;
	bool enableInput;
	uint8_t fadeOffset;
	sf::Time refresh;
	std::shared_ptr<Action> action;

public:
	ActGameFade(const sf::Color& color_, bool isFadeOut_, bool enableInput_,
		uint8_t fadeOffset_, const sf::Time& refresh_) noexcept : color(color_),
		isFadeOut(isFadeOut_), enableInput(enableInput_), fadeOffset(fadeOffset_),
		refresh(refresh_), action(nullptr) {}

	void setAction(const std::shared_ptr<Action>& action_) noexcept { action = action_; }

	virtual bool execute(Game& game)
	{
		auto fade = game.getFadeInOut();
		if (fade == nullptr)
		{
			auto newFade = std::make_unique<FadeInOut>(game.DrawRegionSizef(),
				isFadeOut,
				enableInput,
				color,
				fadeOffset,
				refresh,
				action);

			game.setFadeInOut(std::move(newFade));
		}
		else
		{
			fade->Reset(
				isFadeOut,
				enableInput,
				color,
				fadeOffset,
				refresh,
				action);
		}
		return true;
	}
};
