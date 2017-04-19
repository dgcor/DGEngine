#include "Menu.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

void Menu::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action)
{
	switch (nameHash16)
	{
	case str2int16("scrollDown"):
		scrollDownAction = action;
		return;
	case str2int16("scrollUp"):
		scrollUpAction = action;
		return;
	}
}

void Menu::updateVisibleItems()
{
	if (items.empty() == true ||
		visibleItems == 0)
	{
		start = 0;
		end = items.size();
		return;
	}

	if (start == 0 && end == 0)
	{
		end = std::min(visibleItems, items.size());
	}
	else if (currentIdx == 0)
	{
		start = 0;
		end = std::min(visibleItems, items.size());
	}
	else if (currentIdx == items.size() - 1)
	{
		if (visibleItems < items.size())
		{
			start = items.size() - visibleItems;
		}
		else
		{
			start = 0;
		}
		end = items.size();
	}
	else if (currentIdx == end && end < items.size())
	{
		start++;
		end++;
	}
	else if (currentIdx < start && start > 0)
	{
		start--;
		end--;
	}
}

void Menu::calculatePositions()
{
	if (recalculatePos == false)
	{
		return;
	}
	recalculatePos = false;

	updateVisibleItems();

	if (items.empty() == true)
	{
		return;
	}

	auto currPos = pos;
	size_t i = 0;
	for (; i < start; i++)
	{
		items[i]->Position(currPos);
	}
	float lastSize = 0;
	for (; i < end; i++)
	{
		const auto& item = items[i];
		item->Position(currPos);
		lastSize = item->Size().y;
		if (i + 1 < end)
		{
			currPos.y += lastSize + verticalPad;
		}
	}
	for (; i < items.size(); i++)
	{
		items[i]->Position(currPos);
	}
	currPos.y += lastSize;
	if (vertAlign != VerticalAlign::Bottom)
	{
		auto offset = pos.y - currPos.y;
		if (vertAlign == VerticalAlign::Center)
		{
			offset = std::round(offset / 2);
		}
		for (auto& item : items)
		{
			auto itemPos = item->Position();
			itemPos.y += offset;
			item->Position(itemPos);
		}
	}
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (visible == false)
	{
		return;
	}
	for (size_t i = start; i < end; i++)
	{
		target.draw(*items[i].get(), states);
	}
}

void Menu::updateSize(const Game& game)
{
	if (game.StretchToFit() == true)
	{
		return;
	}
	auto size = Size();
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldWindowSize(), game.WindowSize());
	GameUtils::setAnchorPosSize(anchor, scrollPosition, size, game.OldWindowSize(), game.WindowSize());

	scrollRect.left = (int)scrollPosition.x;
	scrollRect.top = (int)scrollPosition.y;

	for (const auto& item : items)
	{
		item->updateSize(game);
	}
}

void Menu::update(Game& game)
{
	if (visible == false)
	{
		return;
	}
	auto focus = game.Resources().getFocused();
	for (size_t i = 0; i < items.size(); i++)
	{
		auto button = items[i].get();
		if (button == focus)
		{
			if (recalculateVisiblePos == false)
			{
				recalculateVisiblePos = (currentIdx != i);
			}
			currentIdx = i;
		}
		if (i >= start && i < end)
		{
			button->update(game);
		}
	}
	if ((recalculatePos == true ||
		recalculateVisiblePos == true) &&
		visibleItems > 0)
	{
		recalculatePos = true;
		calculatePositions();
		recalculateVisiblePos = false;
	}

	if (game.wasMouseScrolled() == true)
	{
		const auto& scroll = game.getMouseWheelScroll();
		if (scrollRect.contains(scroll.x, scroll.y))
		{
			game.clearMouseScrolled();
			if (scroll.delta < 0.f)
			{
				if (scrollDownAction != nullptr)
				{
					game.Events().addBack(scrollDownAction);
				}
			}
			else
			{
				if (scrollUpAction != nullptr)
				{
					game.Events().addBack(scrollUpAction);
				}
			}
		}
	}
}

bool Menu::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first.c_str());
	switch (propHash)
	{
	case str2int16("itemCount"):
		var = Variable((int64_t)getItemCount());
		break;
	case str2int16("currentIdx"):
		var = Variable((int64_t)currentIdx);
		break;
	case str2int16("visibleItems"):
		var = Variable((int64_t)visibleItems);
		break;
	default:
		return GameUtils::getUIObjProp(*this, propHash, props.second, var);
	}
	return true;
}
