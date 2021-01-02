#include "Menu.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils/Utils.h"

void Menu::addItem(const std::shared_ptr<StringButton>& item)
{
	items.push_back(item);
	if (vertAlign != VerticalAlign::Bottom)
	{
		recalculatePos = true;
	}
}

StringButton* Menu::getItem() const
{
	return getItem(currentIdx);
}

StringButton* Menu::getItem(size_t idx) const
{
	if (idx < items.size())
	{
		return items[idx].get();
	}
	return nullptr;
}

StringButton* Menu::getVisibleItem(size_t idx) const
{
	return getItem(start + idx);
}

size_t Menu::getItemPosition(const Button* item) const
{
	for (size_t i = 0; i < items.size(); i++)
	{
		if (items[i].get() == item)
		{
			return i;
		}
	}
	return 0;
}

void Menu::ScrollPosition(const sf::Vector2f& position_) noexcept
{
	scrollPosition = position_;
	scrollRect.left = (int)position_.x;
	scrollRect.top = (int)position_.y;
}

void Menu::setVerticalAlign(const VerticalAlign align) noexcept
{
	if (vertAlign != align)
	{
		vertAlign = align;
		recalculatePos = true;
	}
}

void Menu::setVerticalPad(int verticalPad_) noexcept
{
	if (verticalPad != verticalPad_)
	{
		verticalPad = verticalPad_;
		recalculatePos = true;
	}
}

void Menu::setVisibleItems(size_t visibleItems_) noexcept
{
	if (visibleItems == visibleItems_)
	{
		return;
	}
	auto newVal = std::min(visibleItems_, items.size());
	if (visibleItems == 0)
	{
		visibleItems = newVal;
		if (newVal > 0)
		{
			recalculatePos = true;
		}
	}
	else
	{
		if (visibleItems != newVal)
		{
			visibleItems = newVal;
			recalculatePos = true;
		}
	}
}

void Menu::setCurrentIdx(size_t idx) noexcept
{
	if (idx < items.size())
	{
		currentIdx = idx;
		if (idx < start || idx >= end)
		{
			recalculateVisiblePos = true;
		}
	}
}

void Menu::Position(const sf::Vector2f& position_) noexcept
{
	pos = position_;
	if (visibleItems > 0)
	{
		recalculatePos = true;
	}
}

sf::Vector2f Menu::Size() const
{
	return sf::Vector2f((float)scrollRect.width, (float)scrollRect.height);
}

void Menu::Size(const sf::Vector2f& size) noexcept
{
	scrollRect.width = (int)size.x;
	scrollRect.height = (int)size.y;
}

std::shared_ptr<Action> Menu::getAction(uint16_t nameHash16) const noexcept
{
	switch (nameHash16)
	{
	case str2int16("scrollDown"):
		return scrollDownAction;
	case str2int16("scrollUp"):
		return scrollUpAction;
	default:
		return nullptr;
	}
}

bool Menu::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept
{
	switch (nameHash16)
	{
	case str2int16("scrollDown"):
		scrollDownAction = action;
		break;
	case str2int16("scrollUp"):
		scrollUpAction = action;
		break;
	default:
		return false;
	}
	return true;
}

void Menu::updateVisibleItems() noexcept
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

void Menu::draw(const Game& game, sf::RenderTarget& target) const
{
	if (visible == false)
	{
		return;
	}
	for (size_t i = start; i < end; i++)
	{
		items[i]->draw(game, target);
	}
}

void Menu::updateSize(const Game& game)
{
	auto size = Size();
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldDrawRegionSize(), game.DrawRegionSize());
	GameUtils::setAnchorPosSize(anchor, scrollPosition, size, game.OldDrawRegionSize(), game.DrawRegionSize());

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
		if (focusEnable == true)
		{
			if (button == focus)
			{
				if (recalculateVisiblePos == false)
				{
					recalculateVisiblePos = (currentIdx != i);
				}
				currentIdx = i;
			}
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
		const auto& scroll = game.MouseScroll();
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

bool Menu::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
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
	case str2int16("item"):
	{
		auto props2 = Utils::splitStringIn2(props.second, '.');
		auto btnIdx = Utils::strtou(props2.first);
		if (btnIdx < items.size())
		{
			auto button = items[btnIdx].get();
			if (button != nullptr)
			{
				return button->getProperty(props2.second, var);
			}
		}
		return false;
	}
	default:
		return getUIObjProp(propHash, props.second, var);
	}
	return true;
}
