#pragma once

#include "Action.h"
#include "Game.h"
#include <memory>
#include <string>

class ActGameFadeIn : public Action
{
private:
	sf::Color color;
	unsigned refresh;
	uint8_t fadeOffset;
	std::shared_ptr<Action> action;

public:
	ActGameFadeIn(const sf::Color& color_, unsigned refresh_, uint8_t fadeOffset_)
		: color(color_), refresh(refresh_), fadeOffset(fadeOffset_)
	{
		color.a = 255;
	}

	void setAction(const std::shared_ptr<Action>& action_) { action = action_; }

	virtual bool execute(Game& game)
	{
		auto fade = game.getFadeInOut();

		if (fade == nullptr)
		{
			auto newFade = std::make_unique<Rectangle>(game.WindowTexSizef());
			newFade->setFillColor(color);
			game.setFadeInOut(std::move(newFade));
			game.enableInput(false);

			game.Events().addBack(Event(std::make_shared<ActGameFadeIn>(*this), sf::milliseconds(refresh)));
			return true;
		}
		else
		{
			if (color.a == 0)
			{
				game.setFadeInOut(nullptr);
				game.enableInput(true);
				if (action != nullptr)
				{
					game.Events().addBack(action);
				}
				return true;
			}
			else
			{
				if (color.a > fadeOffset)
				{
					color.a -= fadeOffset;
				}
				else
				{
					color.a = 0;
				}
				fade->setFillColor(color);
				return false;
			}
		}
	}
};

class ActGameFadeOut : public Action
{
private:
	sf::Color color;
	unsigned refresh;
	uint8_t fadeOffset;
	std::shared_ptr<Action> action;

public:
	ActGameFadeOut(const sf::Color& color_, unsigned refresh_, uint8_t fadeOffset_)
		: color(color_), refresh(refresh_), fadeOffset(fadeOffset_)
	{
		color.a = 0;
	}

	void setAction(const std::shared_ptr<Action>& action_) { action = action_; }

	virtual bool execute(Game& game)
	{
		auto fade = game.getFadeInOut();

		if (fade == nullptr)
		{
			auto newFade = std::make_unique<Rectangle>(game.WindowTexSizef());
			newFade->setFillColor(color);
			game.setFadeInOut(std::move(newFade));
			game.enableInput(false);

			game.Events().addBack(Event(std::make_shared<ActGameFadeOut>(*this), sf::milliseconds(refresh)));
			return true;
		}
		else
		{
			if (color.a == 255)
			{
				game.setFadeInOut(nullptr);
				game.enableInput(true);
				if (action != nullptr)
				{
					game.Events().addBack(action);
				}
				return true;
			}
			else
			{
				if ((int)color.a + (int)fadeOffset < 255)
				{
					color.a += fadeOffset;
				}
				else
				{
					color.a = 255;
				}
				fade->setFillColor(color);
				return false;
			}
		}
	}
};
