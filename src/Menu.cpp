#include "Menu.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

using Utils::str2int;

void Menu::calculatePositions()
{
	if (recalculatePos == false)
	{
		return;
	}
	recalculatePos = false;
	if (items.empty() == true)
	{
		return;
	}
	if (visibleItems == 0)
	{
		start = 0;
		end = items.size();
	}
	else
	{
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
	if (visibleItems > 0)
	{
		for (size_t i = start; i < end; i++)
		{
			target.draw(*items[i].get(), states);
		}
	}
	else
	{
		for (const auto& item : items)
		{
			target.draw(*item, states);
		}
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

	for (size_t i = 0; i < items.size(); i++)
	{
		items[i]->updateSize(game);
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
		if (visibleItems == 0 ||
			(visibleItems > 0 && i >= start && i < end))
		{
			button->update(game);
		}
		button->update(game);
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
	if (prop.size() > 1)
	{
		auto props = Utils::splitString(prop, '.');
		if (props.size() > 0)
		{
			auto propHash = str2int(props[0].c_str());
			switch (propHash)
			{
			case str2int("itemCount"):
				var = Variable((int64_t)this->getItemCount());
				break;
			default:
				return GameUtils::getUIObjProp(*this, propHash, props, var);
			}
			return true;
		}
	}
	return false;
}
