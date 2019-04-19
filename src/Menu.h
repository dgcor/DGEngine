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

	virtual const sf::Vector2f& DrawPosition() const noexcept { return pos; }
	virtual const sf::Vector2f& Position() const noexcept { return pos; }
	virtual void Position(const sf::Vector2f& position_) noexcept
	{
		pos = position_;
		if (visibleItems > 0)
		{
			recalculatePos = true;
		}
	}
	virtual void ScrollPosition(const sf::Vector2f& position_) noexcept
	{
		scrollPosition = position_;
		scrollRect.left = (int)position_.x;
		scrollRect.top = (int)position_.y;
	}
	virtual sf::Vector2f Size() const
	{
		return sf::Vector2f((float)scrollRect.width, (float)scrollRect.height);
	}
	virtual void Size(const sf::Vector2f& size) noexcept
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

	virtual std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept;
	virtual bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept;

	virtual Anchor getAnchor() const { return anchor; }
	virtual void setAnchor(const Anchor anchor_) noexcept
	{
		if (anchor != anchor_)
		{
			anchor = anchor_;
		}
	}
	virtual void updateSize(const Game& game);

	virtual bool Visible() const noexcept { return visible; }
	virtual void Visible(bool visible_) noexcept { visible = visible_; }

	virtual void draw(const Game& game, sf::RenderTarget& target) const;

	virtual void update(Game& game);

	virtual bool getProperty(const std::string_view prop, Variable& var) const;
};
