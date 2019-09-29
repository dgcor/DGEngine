#pragma once

#include "StringButton.h"
#include <vector>

class Menu : public UIObject
{
private:
	std::vector<std::shared_ptr<StringButton>> items;
	int verticalPad{ 0 };
	size_t visibleItems{ 0 };
	size_t currentIdx{ 0 };
	sf::Vector2f pos;
	VerticalAlign vertAlign{ VerticalAlign::Bottom };
	Anchor anchor{ Anchor::Top | Anchor::Left };
	bool recalculatePos{ false };
	bool recalculateVisiblePos{ true };
	size_t start{ 0 };
	size_t end{ 0 };
	sf::Vector2f scrollPosition;
	sf::IntRect scrollRect;
	std::shared_ptr<Action> scrollDownAction;
	std::shared_ptr<Action> scrollUpAction;
	bool visible{ true };

public:
	void updateVisibleItems() noexcept;
	void calculatePositions();

	void addItem(const std::shared_ptr<StringButton>& item)
	{
		items.push_back(item);
		if (vertAlign != VerticalAlign::Bottom)
		{
			recalculatePos = true;
		}
	}
	StringButton* getItem(size_t idx) const
	{
		if (idx < items.size())
		{
			return items[idx].get();
		}
		return nullptr;
	}
	StringButton* getVisibleItem(size_t idx) const
	{
		return getItem(start + idx);
	}
	size_t getItemCount() const noexcept { return items.size(); }
	size_t getItemPosition(const Button* item) const
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

	const sf::Vector2f& DrawPosition() const noexcept override { return pos; }
	const sf::Vector2f& Position() const noexcept override { return pos; }
	void Position(const sf::Vector2f& position_) noexcept override
	{
		pos = position_;
		if (visibleItems > 0)
		{
			recalculatePos = true;
		}
	}
	void ScrollPosition(const sf::Vector2f& position_) noexcept
	{
		scrollPosition = position_;
		scrollRect.left = (int)position_.x;
		scrollRect.top = (int)position_.y;
	}
	sf::Vector2f Size() const override
	{
		return sf::Vector2f((float)scrollRect.width, (float)scrollRect.height);
	}
	void Size(const sf::Vector2f& size) noexcept override
	{
		scrollRect.width = (int)size.x;
		scrollRect.height = (int)size.y;
	}

	void setVerticalAlign(const VerticalAlign align) noexcept
	{
		if (vertAlign != align)
		{
			vertAlign = align;
			recalculatePos = true;
		}
	}
	void setVerticalPad(int verticalPad_) noexcept
	{
		if (verticalPad != verticalPad_)
		{
			verticalPad = verticalPad_;
			recalculatePos = true;
		}
	}
	void setVisibleItems(size_t visibleItems_) noexcept
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

	void setCurrentIdx(size_t idx) noexcept
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

	std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept override;
	bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept override;

	Anchor getAnchor() const override { return anchor; }
	void setAnchor(const Anchor anchor_) noexcept override
	{
		if (anchor != anchor_)
		{
			anchor = anchor_;
		}
	}
	void updateSize(const Game& game) override;

	bool Visible() const noexcept override { return visible; }
	void Visible(bool visible_) noexcept override { visible = visible_; }

	void draw(const Game& game, sf::RenderTarget& target) const override;

	void update(Game& game) override;

	bool getProperty(const std::string_view prop, Variable& var) const override;
};
