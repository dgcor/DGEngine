#include "ItemCollection.h"

ItemCollection::ItemCollection(const sf::Vector2u& size_) : size(size_), maxItemSize(size_)
{
	items.resize(size_.x * size.y);
}

bool ItemCollection::set(const std::shared_ptr<Item>& item,
	const sf::Vector2u& position, std::shared_ptr<Item>& oldItem)
{
	if (allowedItemClasses.empty() == false
		&& std::find(
			allowedItemClasses.begin(),
			allowedItemClasses.end(),
			item->Class()
		) == allowedItemClasses.end())
	{
		return false;
	}
	auto itemSize = item->InventorySize();
	if (itemSize.x > maxItemSize.x
		|| itemSize.y > maxItemSize.y)
	{
		return false;
	}
	auto pos = position;
	auto posEndX = pos.x + itemSize.x;
	auto posEndY = pos.y + itemSize.y;
	if (posEndX > size.x)
	{
		posEndX = size.x;
		pos.x = size.x - itemSize.x;
	}
	if (posEndY > size.y)
	{
		posEndY = size.y;
		pos.y = size.y - itemSize.y;
	}
	std::shared_ptr<Item> itemTemp;
	for (size_t i = pos.x; i < posEndX; i++)
	{
		for (size_t j = pos.x; j < posEndX; j++)
		{
			const auto& item = get(i, j);
			if (item != nullptr)
			{
				if (itemTemp == nullptr)
				{
					itemTemp = item;
				}
				else
				{
					return false;
				}
			}
		}
	}
	oldItem = itemTemp;
	items[pos.x + pos.y * size.y] = item;
	return true;
}
