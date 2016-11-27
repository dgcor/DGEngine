#include "ItemCollection.h"
#include "Utils.h"

ItemCollection::ItemCollection(size_t size_) : size(size_, 0)
{
	items.resize(size_);
}

ItemCollection::ItemCollection(const sf::Vector2u& size_) : size(size_)
{
	items.resize(size_.x * size_.y);
}

void ItemCollection::init(size_t size_)
{
	size = sf::Vector2u(size_, 0);
	items.resize(size_);
}

void ItemCollection::init(const sf::Vector2u& size_)
{
	size = size_;
	items.resize(size_.x * size_.y);
}

void ItemCollection::allowType(const std::string& type)
{
	auto typeHash = Utils::str2int(type.c_str());
	if (std::find(allowedTypes.begin(), allowedTypes.end(), typeHash) == allowedTypes.end())
	{
		allowedTypes.push_back(typeHash);
	}
}

bool ItemCollection::isTypeAllowed(const std::string& type) const
{
	return isTypeAllowed(Utils::str2int(type.c_str()));
}

bool ItemCollection::isTypeAllowed(unsigned typeHash) const
{
	if (allowedTypes.empty() == true)
	{
		return true;
	}
	return (std::find(allowedTypes.begin(), allowedTypes.end(), typeHash) != allowedTypes.end());
}

void ItemCollection::set(size_t idx, const std::shared_ptr<Item>& item)
{
	if (idx < items.size())
	{
		items[idx] = item;
	}
}

void ItemCollection::set(const sf::Vector2u& position, const std::shared_ptr<Item>& item)
{
	auto pos = position;

	auto itemSize = item->InventorySize();
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
					return;
				}
			}
		}
	}
	items[pos.x + pos.y * size.y] = item;
}
