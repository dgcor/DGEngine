#include "ItemCollection.h"
#include "Utils/Utils.h"

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
	resetIndexes();
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
	resetIndexes();
}

void ItemCollection::resetIndexes() noexcept
{
	for (auto& item : items)
	{
		item.second = -1;
	}
}

void ItemCollection::allowType(const std::string& type)
{
	auto typeHash = str2int16(type.c_str());
	if (std::find(allowedTypes.begin(), allowedTypes.end(), typeHash) == allowedTypes.end())
	{
		allowedTypes.push_back(typeHash);
	}
}

bool ItemCollection::isTypeAllowed(const std::string& type) const
{
	return isTypeAllowed(str2int16(type.c_str()));
}

bool ItemCollection::isTypeAllowed(uint16_t typeHash16) const
{
	if (allowedTypes.empty() == true)
	{
		return true;
	}
	return (std::find(allowedTypes.begin(), allowedTypes.end(), typeHash16) != allowedTypes.end());
}

Item* ItemCollection::get(size_t idx) const
{
	if (idx < items.size())
	{
		if (items[idx].second >= 0 &&
			((size_t)items[idx].second) < items.size())
		{
			return items[(size_t)items[idx].second].first.get();
		}
		return items[idx].first.get();
	}
	return nullptr;
}

bool ItemCollection::set(size_t idx, std::unique_ptr<Item>& item)
{
	std::unique_ptr<Item> oldItem;
	return set(idx, item, oldItem);
}

bool ItemCollection::set(size_t idx, std::unique_ptr<Item>& item,
	std::unique_ptr<Item>& oldItem)
{
	if (idx < items.size())
	{
		if (item != nullptr &&
			isTypeAllowed(item->Class()->TypeHash16()) == false)
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

bool ItemCollection::set(const ItemXY& position, std::unique_ptr<Item>& item)
{
	std::unique_ptr<Item> oldItem;
	return set(position, item, oldItem);
}

bool ItemCollection::set(const ItemXY& position,
	std::unique_ptr<Item>& item, std::unique_ptr<Item>& oldItem)
{
	size_t idx = position.x + position.y * size.x;
	if (idx < items.size())
	{
		if (item != nullptr &&
			isTypeAllowed(item->Class()->TypeHash16()) == false)
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
	std::unique_ptr<Item>& item, std::unique_ptr<Item>& oldItem)
{
	oldItem = std::move(items[idx].first);
	items[idx].first = std::move(item);
	items[idx].second = -1;
	return true;
}

bool ItemCollection::setAndEnforceItemSize(const ItemXY& position,
	std::unique_ptr<Item>& item, std::unique_ptr<Item>& oldItem)
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
	Item* oldItemTemp = nullptr;
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
		int32_t oldIdx = -1;
		for (size_t i = 0; i < items.size(); i++)
		{
			if (oldIdx >= 0)
			{
				if (items[i].second == oldIdx)
				{
					items[i].second = -1;
				}
				continue;
			}
			if (items[i].first.get() == oldItemTemp)
			{
				oldIdx = (int32_t)i;
				oldItem = std::move(items[i].first);
			}
		}
	}
	else
	{
		oldItem = nullptr;
	}

	int32_t newIdx = -1;
	for (size_t i = pos.x; i < posEndX; i++)
	{
		for (size_t j = pos.y; j < posEndY; j++)
		{
			size_t idx = i + j * size.x;
			if (newIdx >= 0)
			{
				items[idx].second = newIdx;
				continue;
			}
			items[idx].first = std::move(item);
			items[idx].second = -1;
			newIdx = (int32_t)idx;
		}
	}
	return true;
}

bool ItemCollection::isFull() const noexcept
{
	for (const auto& item : items)
	{
		if (item.first == nullptr &&
			item.second < 0)
		{
			return false;
		}
	}
	return true;
}

bool ItemCollection::isItemSlotInUse(size_t idx) const
{
	if (idx >= items.size())
	{
		return false;
	}
	return items[idx].first != nullptr;
}

bool ItemCollection::isItemSlotInUse(const ItemXY& position) const
{
	if (position.x >= size.x
		|| position.y >= size.y)
	{
		return false;
	}
	return isItemSlotInUse(position.x + position.y * size.x);
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
			size_t idx = i + j * size.x;
			if (items[idx].first != nullptr ||
				items[idx].second >= 0)
			{
				return false;
			}
		}
	}
	itemIdx = (size_t)(x + y * (int)size.x);
	return true;
}

bool ItemCollection::getFreeItemSlot(const Item& item,
	size_t& itemIdx, InventoryPosition invPos) const
{
	if (isTypeAllowed(item.Class()->TypeHash16()) == false)
	{
		return false;
	}

	const auto& itemSize = item.Class()->InventorySize();

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

bool ItemCollection::hasFreeItemSlot(const Item& item) const
{
	size_t itemIdx;
	return getFreeItemSlot(item, itemIdx);
}

bool ItemCollection::find(uint16_t itemTypeHash16,
	size_t& idx, Item*& item) const
{
	auto size = items.size();
	if (idx < size)
	{
		for (size_t i = idx; i < size; i++)
		{
			if (isItemSlotInUse(i) == true)
			{
				if (items[i].first->Class()->TypeHash16() == itemTypeHash16)
				{
					idx = i;
					item = items[i].first.get();
					return true;
				}
			}
		}
	}
	idx = size;
	return false;
}

unsigned ItemCollection::countFreeSlots(const ItemClass& itemClass) const
{
	// only for items whose size is 1
	unsigned count = 0;
	if (isTypeAllowed(itemClass.TypeHash16()) == true)
	{
		for (const auto& item : items)
		{
			if (item.first == nullptr || item.second < 0)
			{
				count++;
			}
		}
	}
	return count;
}

bool ItemCollection::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	switch (str2int16(props.first.c_str()))
	{
	case str2int16("capacity"):
		var = Variable((int64_t)items.size());
		break;
	case str2int16("enforceItemSize"):
		var = Variable((bool)enforceItemSize);
		break;
	case str2int16("isFull"):
		var = Variable((bool)isFull());
		break;
	case str2int16("size"):
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
