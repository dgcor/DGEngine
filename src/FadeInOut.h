#pragma once

#include "Anchor.h"
#include "Actions/Action.h"
#include <memory>
#include "Rectangle.h"

class FadeInOut : public Rectangle
{
private:
	bool isFadeOut;
	bool enableInput;
	uint8_t fadeOffset;
	sf::Time frameTime;
	sf::Time currentTime;
	std::shared_ptr<Action> action;

	bool updateEnableInput{ false };

	bool HasFadeEnded(uint8_t alpha);
	void UpdateFade(sf::Color& color);

public:
	FadeInOut(const sf::Vector2f& size_, bool isFadeOut_, bool enableInput_, sf::Color color_,
		uint8_t fadeOffset_, const sf::Time& frameTime_, const std::shared_ptr<Action>& action_)
		: Rectangle(size_), isFadeOut(isFadeOut_), enableInput(enableInput_),
		fadeOffset(fadeOffset_), frameTime(frameTime_), action(action_)
	{
		if (enableInput_ == false)
		{
			updateEnableInput = true;
		}
		if (isFadeOut_ == true)
		{
			color_.a = 0;
		}
		else
		{
			color_.a = 255;
		}
		setFillColor(color_);
	}

	void Reset(bool isFadeOut_, bool enableInput_, sf::Color color_, uint8_t fadeOffset_,
		const sf::Time& frameTime_, const std::shared_ptr<Action>& action_)
	{
		if (enableInput != enableInput_)
		{
			updateEnableInput = true;
		}
		isFadeOut = isFadeOut_;
		enableInput = enableInput_;
		fadeOffset = fadeOffset_;
		frameTime = frameTime_;
		action = action_;

		color_.a = getFillColor().a;
		setFillColor(color_);
	}

	virtual void update(Game& game);
};
