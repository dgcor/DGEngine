#include "ScrollableText.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

ScrollableText::ScrollableText(std::unique_ptr<DrawableText> text_, const sf::Time& frameTime_) :
	text(std::move(text_)), frameTime(frameTime_)
{
	text->Position(sf::Vector2f(0.f, 0.f));
	text->setAnchor(Anchor::None);
	text->Visible(true);
}

void ScrollableText::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action)
{
	switch (nameHash16)
	{
	case str2int16("complete"):
		completeAction = action;
		return;
	}
}

void ScrollableText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (visible == true)
	{
		auto origView = target.getView();
		target.setView(view.getView());
		target.draw(*text);
		target.setView(origView);
	}
}

void ScrollableText::updateSize(const Game& game)
{
	view.updateSize(game);
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

			game.Events().addBack(completeAction);
		}

		view.setCenter(rect);
	}
}

bool ScrollableText::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	return GameUtils::getUIObjProp(*this, str2int16(props.first.c_str()), props.second, var);
}
