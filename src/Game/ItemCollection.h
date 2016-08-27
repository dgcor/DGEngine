#pragma once

#include "Item.h"
#include <memory>
#include <vector>

class ItemCollection
{
private:
	std::vector<std::shared_ptr<Item>> items;
	sf::Vector2u size;
	sf::Vector2u maxItemSize;
	std::vector<std::string> allowedItemClasses;

public:
	ItemCollection() {}
	ItemCollection(const sf::Vector2u& size_);

	void setAllowedItemClasses(const std::vector<std::string>& itemClasses)
	{
		allowedItemClasses = itemClasses;
	}

	void setMaxItemSize(const sf::Vector2u& size_)
	{
		maxItemSize.x = std::min(size_.x, size.x);
		maxItemSize.y = std::min(size_.y, size.y);
	}

	const std::shared_ptr<Item>& get(size_t x, size_t y) const { return items[x + y * size.y]; }
	const std::shared_ptr<Item>& get(const sf::Vector2u& pos) const { return get(pos.x, pos.y); }

	bool set(const std::shared_ptr<Item>& item,
		const sf::Vector2u& position, std::shared_ptr<Item>& oldItem);
};
