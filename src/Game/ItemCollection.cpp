#include "ItemCollection.h"
#include "Utils.h"

ItemCollection::ItemCollection(size_t size_)
{
	init(size_);
}

ItemCollection::ItemCollection(const ItemXY& size_)
{
	init(size_);
}

void ItemCollection::init(size_t size_)
{
	uint8_t newSize = (size_ > 0xFF ? 0xFF : (uint8_t)size_);
	size = ItemXY(newSize, 1);
	items.resize(newSize);
}

void ItemCollection::init(const ItemXY& size_)
{
	if (size_.x * size_.y > 1024)
	{
		size.x = 32;
		size.y = 32;
	}
	else
	{
		size = size_;
	}
	items.resize(size.x * size.y);
}

void ItemCollection::allowType(const std::string& type)
{
	auto typeHash = str2int32(type.c_str());
	if (std::find(allowedTypes.begin(), allowedTypes.end(), typeHash) == allowedTypes.end())
	{
		allowedTypes.push_back(typeHash);
	}
}

bool ItemCollection::isTypeAllowed(const std::string& type) const
{
	return isTypeAllowed(str2int32(type.c_str()));
}

bool ItemCollection::isTypeAllowed(uint32_t typeHash) const
{
	if (allowedTypes.empty() == true)
	{
		return true;
	}
	return (std::find(allowedTypes.begin(), allowedTypes.end(), typeHash) != allowedTypes.end());
}

bool ItemCollection::set(size_t idx, const std::shared_ptr<Item>& item)
{
	std::shared_ptr<Item> oldItem;
	return set(idx, item, oldItem);
}

bool ItemCollection::set(size_t idx, const std::shared_ptr<Item>& item,
	std::shared_ptr<Item>& oldItem)
{
	if (idx < items.size())
	{
		if (item != nullptr &&
			isTypeAllowed(item->Class()->TypeHash()) == false)
		{
			return false;
		}
		if (enforceItemSize == false)
		{
			return setAndDontEnforceItemSize(idx, item, oldItem);
		}
		else
		{
			return setAndEnforceItemSize(
				ItemXY((uint8_t)(idx % size.x), (uint8_t)(idx / size.x)),
				item,
				oldItem);
		}
	}
	return false;
}

bool ItemCollection::set(const ItemXY& position, const std::shared_ptr<Item>& item)
{
	std::shared_ptr<Item> oldItem;
	return set(position, item, oldItem);
}

bool ItemCollection::set(const ItemXY& position,
	const std::shared_ptr<Item>& item, std::shared_ptr<Item>& oldItem)
{
	size_t idx = position.x + position.y * size.x;
	if (idx < items.size())
	{
		if (item != nullptr &&
			isTypeAllowed(item->Class()->TypeHash()) == false)
		{
			return false;
		}
		if (enforceItemSize == false)
		{
			return setAndDontEnforceItemSize(idx, item, oldItem);
		}
		else
		{
			return setAndEnforceItemSize(position, item, oldItem);
		}
	}
	return false;
}

bool ItemCollection::setAndDontEnforceItemSize(size_t idx,
	const std::shared_ptr<Item>& item, std::shared_ptr<Item>& oldItem)
{
	oldItem = items[idx];
	items[idx] = item;
	return true;
}

bool ItemCollection::setAndEnforceItemSize(const ItemXY& position,
	const std::shared_ptr<Item>& item, std::shared_ptr<Item>& oldItem)
{
	if (size.x == 0 || size.y == 0)
	{
		return false;
	}
	ItemXY itemSize(1, 1);
	if (item != nullptr)
	{
		itemSize = item->Class()->InventorySize();
		if (itemSize.x > size.x
			|| itemSize.y > size.y)
		{
			return false;
		}
	}
	auto pos = position;
	size_t posEndX = (size_t)(pos.x + itemSize.x);
	size_t posEndY = (size_t)(pos.y + itemSize.y);
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
	std::shared_ptr<Item> oldItemTemp;
	for (size_t i = pos.x; i < posEndX; i++)
	{
		for (size_t j = pos.y; j < posEndY; j++)
		{
			const auto& item2 = get(i, j);
			if (item2 != nullptr)
			{
				if (oldItemTemp == nullptr)
				{
					oldItemTemp = item2;
				}
				else if (oldItemTemp != item2)
				{
					return false;
				}
			}
		}
	}
	if (oldItemTemp != nullptr)
	{
		for (size_t i = 0; i < items.size(); i++)
		{
			if (items[i] == oldItemTemp)
			{
				items[i] = nullptr;
			}
		}
	}
	oldItem = oldItemTemp;
	for (size_t i = pos.x; i < posEndX; i++)
	{
		for (size_t j = pos.y; j < posEndY; j++)
		{
			items[i + j * size.x] = item;
		}
	}
	return true;
}

bool ItemCollection::isFull() const
{
	for (const auto& item : items)
	{
		if (item == nullptr)
		{
			return false;
		}
	}
	return true;
}

bool ItemCollection::isItemSlotInUse(size_t idx) const
{
	if (idx < items.size())
	{
		return isItemSlotInUse(ItemXY((uint8_t)(idx % size.x), (uint8_t)(idx / size.x)));
	}
	return false;
}

bool ItemCollection::isItemSlotInUse(const ItemXY& position) const
{
	if (position.x >= size.x
		|| position.y >= size.y)
	{
		return false;
	}
	auto item = get(position).get();
	if (item == nullptr)
	{
		return false;
	}
	bool isPrevItemSameX = false;
	bool isPrevItemSameY = false;
	if (position.x > 0)
	{
		isPrevItemSameX = (get(position.x - 1, position.y).get() == item);
	}
	if (position.y > 0)
	{
		isPrevItemSameY = (get(position.x, position.y - 1).get() == item);
	}
	if (isPrevItemSameX || isPrevItemSameY)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool ItemCollection::isItemSlotEmpty(int x, int y,
	const ItemXY& itemSize, size_t& itemIdx) const
{
	size_t sizeX = (size_t)x + itemSize.x;
	size_t sizeY = (size_t)y + itemSize.y;

	for (size_t i = x; i < sizeX; i++)
	{
		for (size_t j = y; j < sizeY; j++)
		{
			if (get(i, j).get() != nullptr)
			{
				return false;
			}
		}
	}
	itemIdx = (size_t)(x + y * (int)size.x);
	return true;
}

bool ItemCollection::getItemSlot(const ItemXY& itemSize,
	size_t& itemIdx, InventoryPosition invPos) const
{
	if (itemSize.x > size.x || itemSize.y > size.y)
	{
		return false;
	}

	int sizeX = (int)size.x - (int)itemSize.x;
	int sizeY = (int)size.y - (int)itemSize.y;

	switch (invPos)
	{
	default:
	case InventoryPosition::TopLeft:
	{
		for (int y = 0; y <= sizeY; y++)
		{
			for (int x = 0; x <= sizeX; x++)
			{
				if (isItemSlotEmpty(x, y, itemSize, itemIdx) == true)
				{
					return true;
				}
			}
		}
		break;
	}
	case InventoryPosition::TopRight:
	{
		for (int y = 0; y <= sizeY; y++)
		{
			for (int x = sizeX; x >= 0; x--)
			{
				if (isItemSlotEmpty(x, y, itemSize, itemIdx) == true)
				{
					return true;
				}
			}
		}
		break;
	}
	case InventoryPosition::BottomLeft:
	{
		for (int y = sizeY; y >= 0; y--)
		{
			for (int x = 0; x <= sizeX; x++)
			{
				if (isItemSlotEmpty(x, y, itemSize, itemIdx) == true)
				{
					return true;
				}
			}
		}
		break;
	}
	case InventoryPosition::BottomRight:
	{
		for (int y = sizeY; y >= 0; y--)
		{
			for (int x = sizeX; x >= 0; x--)
			{
				if (isItemSlotEmpty(x, y, itemSize, itemIdx) == true)
				{
					return true;
				}
			}
		}
		break;
	}
	}
	return false;
}

bool ItemCollection::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	switch (str2int32(props.first.c_str()))
	{
	case str2int32("capacity"):
		var = Variable((int64_t)items.size());
		break;
	case str2int32("enforceItemSize"):
		var = Variable((bool)enforceItemSize);
		break;
	case str2int32("isFull"):
		var = Variable((bool)isFull());
		break;
	case str2int32("size"):
	{
		if (props.second == "x")
		{
			var = Variable((int64_t)size.x);
		}
		else
		{
			var = Variable((int64_t)size.y);
		}
	}
	break;
	default:
		return false;
	}
	return true;
}
