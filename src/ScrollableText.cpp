#include "ScrollableText.h"
#include "Game.h"
#include "GameUtils.h"
#include "SFMLUtils.h"
#include "Utils.h"

ScrollableText::ScrollableText(std::unique_ptr<DrawableText> text_, const sf::Time& frameTime_) :
	text(std::move(text_)), frameTime(frameTime_)
{
	text->Position(sf::Vector2f(0.f, 0.f));
	text->setAnchor(Anchor::None);
	text->Visible(true);
}

void ScrollableText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (visible == true)
	{
		auto origView = target.getView();
		target.setView(view);
		target.draw(*text);
		target.setView(origView);
	}
}

void ScrollableText::updateViewPort(const Game& game)
{
	view.setViewport(game);
}

void ScrollableText::updateSize(const Game& game)
{
	if (game.StretchToFit() == true)
	{
		view.setViewport(game);
		return;
	}
	auto pos = view.getPosition();
	auto size = view.getSize();
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldWindowSize(), game.WindowSize());
	view.setPosition(pos);
	view.setSize(size);
	view.setViewport(game);
}

void ScrollableText::update(Game& game)
{
	if (pause == true || visible == false)
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

		auto rect = view.getCenter();
		auto yy = (int)text->Size().y;
		if (rect.y - (height / 2) < yy)
		{
			rect.y++;
		}
		else
		{
			rect.y = -(float)height;

			if (loop == false)
			{
				pause = true;
			}

			game.Events().addBack(action);
		}

		view.setCenter(rect);
	}
}

bool ScrollableText::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.size() > 1)
	{
		auto props = Utils::splitString(prop, '.');
		if (props.size() > 0)
		{
			return GameUtils::getUIObjProp(*this, Utils::str2int(props[0].c_str()), props, var);
		}
	}
	return false;
}
