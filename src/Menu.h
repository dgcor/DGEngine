#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "StringButton.h"
#include "UIObject.h"

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
		if (idx > 0 && idx <= items.size())
		{
			return items[idx - 1].get();
		}
		return nullptr;
	}
	size_t getItemCount() const { return items.size(); }
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

	virtual const sf::Vector2f& DrawPosition() const { return pos; }
	virtual const sf::Vector2f& Position() const { return pos; }
	virtual void Position(const sf::Vector2f& position_)
	{
		pos = position_;
		if (visibleItems > 0)
		{
			recalculatePos = true;
		}
	}
	virtual void ScrollPosition(const sf::Vector2f& position_)
	{
		scrollPosition = position_;
		scrollRect.left = (int)position_.x;
		scrollRect.top = (int)position_.y;
	}
	virtual sf::Vector2f Size() const
	{
		return sf::Vector2f((float)scrollRect.width, (float)scrollRect.height);
	}
	virtual void Size(const sf::Vector2f& size)
	{
		scrollRect.width = (int)size.x;
		scrollRect.height = (int)size.y;
	}

	void setScrollDownAction(const std::shared_ptr<Action>& action) { scrollDownAction = action; }
	void setScrollUpAction(const std::shared_ptr<Action>& action) { scrollUpAction = action; }
	void setVerticalAlign(const VerticalAlign align)
	{
		if (vertAlign != align)
		{
			vertAlign = align;
			recalculatePos = true;
		}
	}
	void setVerticalPad(int verticalPad_)
	{
		if (verticalPad != verticalPad_)
		{
			verticalPad = verticalPad_;
		}
	}
	void setVisibleItems(size_t visibleItems_)
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

	virtual void setAnchor(const Anchor anchor_)
	{
		if (anchor != anchor_)
		{
			anchor = anchor_;
		}
	}
	virtual void updateSize(const Game& game);

	virtual bool Visible() const { return visible; }
	virtual void Visible(bool visible_) { visible = visible_; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual void update(Game& game);

	virtual bool getProperty(const std::string& prop, Variable& var) const;
};
