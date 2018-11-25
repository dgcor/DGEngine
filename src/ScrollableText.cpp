#include "ScrollableText.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils/Utils.h"

void ScrollableText::reset()
{
	text->Position(sf::Vector2f(0.f, 0.f));
	const auto& vSize = view.getVisibleSize();
	view.reset(sf::FloatRect(0, -vSize.y, vSize.x, vSize.y));
	height = vSize.y;
}

ScrollableText::ScrollableText(std::unique_ptr<DrawableText> text_, const sf::Time& frameTime_) :
	text(std::move(text_)), frameTime(frameTime_)
{
	text->Position(sf::Vector2f(0.f, 0.f));
	text->setAnchor(Anchor::None);
	text->Visible(true);
}

std::shared_ptr<Action> ScrollableText::getAction(uint16_t nameHash16) const noexcept
{
	switch (nameHash16)
	{
	case str2int16("complete"):
		return completeAction;
	default:
		return nullptr;
	}
}

bool ScrollableText::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept
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

	currentTime += game.getElapsedTime();

	while (currentTime >= frameTime)
	{
		currentTime -= frameTime;

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

bool ScrollableText::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	return GameUtils::getUIObjProp(*this, str2int16(props.first), props.second, var);
}
