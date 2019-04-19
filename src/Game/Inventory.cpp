#include "Inventory.h"
#include "Game/GameHashes.h"
#include "Utils/Utils.h"

Inventory::Inventory(size_t size_)
{
	init(size_);
}

Inventory::Inventory(const PairUInt8& size_)
{
	init(size_);
}

void Inventory::init(size_t size_)
{
	uint8_t newSize = (size_ > 0xFF ? 0xFF : (uint8_t)size_);
	size = PairUInt8(newSize, 1);
	items.resize(newSize);
	resetIndexes();
}

void Inventory::init(const PairUInt8& size_)
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

void Inventory::resetIndexes() noexcept
{
	for (auto& item : items)
	{
		item.second = -1;
	}
}

bool Inventory::empty() const noexcept
{
	for (const auto& item : items)
	{
		if (item.first != nullptr ||
			item.second >= 0)
		{
			return false;
		}
	}
	return true;
}

void Inventory::allowType(const std::string_view type)
{
	auto typeHash = str2int16(type);
	allowTypeByHash(typeHash);
}

void Inventory::allowTypeByHash(uint16_t typeHash16)
{
	if (std::find(allowedTypes.begin(), allowedTypes.end(), typeHash16) == allowedTypes.end())
	{
		allowedTypes.push_back(typeHash16);
	}
}

bool Inventory::isTypeAllowed(const std::string_view type) const
{
	return isTypeAllowed(str2int16(type));
}

bool Inventory::isTypeAllowed(uint16_t typeHash16) const
{
	if (allowedTypes.empty() == true)
	{
		return true;
	}
	return (std::find(allowedTypes.begin(), allowedTypes.end(), typeHash16) != allowedTypes.end());
}

Item* Inventory::get(size_t idx) const
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

bool Inventory::set(size_t idx, std::unique_ptr<Item>& item)
{
	std::unique_ptr<Item> oldItem;
	return set(idx, item, oldItem);
}

bool Inventory::set(size_t idx, std::unique_ptr<Item>& item,
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
				PairUInt8((uint8_t)(idx % size.x), (uint8_t)(idx / size.x)),
				item,
				oldItem);
		}
	}
	return false;
}

bool Inventory::set(const PairUInt8& position, std::unique_ptr<Item>& item)
{
	std::unique_ptr<Item> oldItem;
	return set(position, item, oldItem);
}

bool Inventory::set(const PairUInt8& position, std::unique_ptr<Item>& item,
	std::unique_ptr<Item>& oldItem)
{
	if (position.x >= size.x || position.y >= size.y)
	{
		return false;
	}
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

bool Inventory::setAndDontEnforceItemSize(size_t idx, std::unique_ptr<Item>& item,
	std::unique_ptr<Item>& oldItem)
{
	Item* itemPtr = item.get();
	if (item != nullptr)
	{
		item->clearMapPosition();
	}
	oldItem = std::move(items[idx].first);
	items[idx].first = std::move(item);
	items[idx].second = -1;

	LevelObjValue transferedQuantity;
	if (updateQuantities(itemPtr, oldItem.get(), transferedQuantity) == true)
	{
		oldItem = nullptr;
	}
	return true;
}

bool Inventory::setAndEnforceItemSize(const PairUInt8& position,
	std::unique_ptr<Item>& item, std::unique_ptr<Item>& oldItem)
{
	if (position.x >= size.x || position.y >= size.y ||
		size.x == 0 || size.y == 0)
	{
		return false;
	}
	PairUInt8 itemSize(1, 1);
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
	Item* itemPtr = item.get();
	Item* oldItemPtr = nullptr;
	for (size_t i = pos.x; i < posEndX; i++)
	{
		for (size_t j = pos.y; j < posEndY; j++)
		{
			const auto& item2 = get(i, j);
			if (item2 != nullptr)
			{
				if (oldItemPtr == nullptr)
				{
					oldItemPtr = item2;
				}
				else if (oldItemPtr != item2)
				{
					return false;
				}
			}
		}
	}
	if (oldItemPtr != nullptr)
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
			if (items[i].first.get() == oldItemPtr)
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
			if (item != nullptr)
			{
				item->clearMapPosition();
			}
			items[idx].first = std::move(item);
			items[idx].second = -1;
			newIdx = (int32_t)idx;
		}
	}

	LevelObjValue transferedQuantity;
	if (updateQuantities(itemPtr, oldItemPtr, transferedQuantity) == true)
	{
		oldItem = nullptr;
	}

	return true;
}

bool Inventory::isFull() const noexcept
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

bool Inventory::isSlotInUse(size_t idx) const
{
	if (idx >= items.size())
	{
		return false;
	}
	return items[idx].first != nullptr;
}

bool Inventory::isSlotInUse(const PairUInt8& position) const
{
	if (position.x >= size.x || position.y >= size.y)
	{
		return false;
	}
	return isSlotInUse(position.x + position.y * size.x);
}

bool Inventory::isSlotEmpty(int x, int y,
	const PairUInt8& itemSize, size_t& itemIdx) const
{
	size_t sizeX = (size_t)x;
	size_t sizeY = (size_t)y;
	if (enforceItemSize == true)
	{
		sizeX += itemSize.x;
		sizeY += itemSize.y;
	}
	else
	{
		sizeX++;
		sizeY++;
	}
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

bool Inventory::getFreeSlot(const Item& item,
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
				if (isSlotEmpty(x, y, itemSize, itemIdx) == true)
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
				if (isSlotEmpty(x, y, itemSize, itemIdx) == true)
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
				if (isSlotEmpty(x, y, itemSize, itemIdx) == true)
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
				if (isSlotEmpty(x, y, itemSize, itemIdx) == true)
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

bool Inventory::hasFreeSlot(const Item& item) const
{
	size_t itemIdx;
	return getFreeSlot(item, itemIdx, InventoryPosition::TopLeft);
}

bool Inventory::hasItem(uint16_t classIdHash16) const
{
	for (const auto& item : (*this))
	{
		if (item.Class()->IdHash16() == classIdHash16)
		{
			return true;
		}
	}
	return false;
}

bool Inventory::findByClass(uint16_t classIdHash16, size_t& idx, Item*& item) const
{
	auto size = items.size();
	if (idx < size)
	{
		for (size_t i = idx; i < size; i++)
		{
			if (isSlotInUse(i) == true)
			{
				if (items[i].first->Class()->IdHash16() == classIdHash16)
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

uint32_t Inventory::findFreeQuantity(
	uint16_t classIdHash16, size_t& idx, Item*& itemFound) const
{
	auto size = items.size();
	if (idx < size)
	{
		for (size_t i = idx; i < size; i++)
		{
			if (isSlotInUse(i) == true)
			{
				if (items[i].first->Class()->IdHash16() == classIdHash16)
				{
					auto itemPtr = items[i].first.get();
					auto itemQuantity = itemPtr->getIntByHash(ItemProp::Quantity);
					auto itemCapacity = itemPtr->getIntByHash(ItemProp::Capacity);
					if (itemQuantity < itemCapacity &&
						itemCapacity > 0)
					{
						idx = i;
						itemFound = itemPtr;
						return (uint32_t)(itemCapacity - itemQuantity);
					}
				}
			}
		}
	}
	idx = size;
	return 0;
}

uint32_t Inventory::findBiggestFreeQuantity(uint16_t classIdHash16,
	Item*& itemFound, uint32_t quantityNeded) const
{
	uint32_t biggestFreeQuantity = 0;
	uint32_t freeQuantity = 0;
	Item* biggestItem = nullptr;
	Item* foundItem = nullptr;
	size_t idx = 0;
	while ((freeQuantity = findFreeQuantity(classIdHash16, idx, foundItem)))
	{
		if (quantityNeded < freeQuantity &&
			biggestFreeQuantity < freeQuantity)
		{
			biggestFreeQuantity = freeQuantity;
			biggestItem = foundItem;
		}
		idx++;
	}
	if (biggestItem != nullptr)
	{
		itemFound = biggestItem;
		return biggestFreeQuantity;
	}
	return 0;
}

uint32_t Inventory::findSmallestFreeQuantity(uint16_t classIdHash16,
	Item*& itemFound, uint32_t quantityNeded) const
{
	uint32_t smallestFreeQuantity = std::numeric_limits<uint32_t>::max();
	uint32_t freeQuantity = 0;
	Item* smallestItem = nullptr;
	Item* foundItem = nullptr;
	size_t idx = 0;
	while ((freeQuantity = findFreeQuantity(classIdHash16, idx, foundItem)))
	{
		if (quantityNeded < freeQuantity &&
			smallestFreeQuantity > freeQuantity)
		{
			smallestFreeQuantity = freeQuantity;
			smallestItem = foundItem;
		}
		idx++;
	}
	if (smallestItem != nullptr)
	{
		itemFound = smallestItem;
		return smallestFreeQuantity;
	}
	return 0;
}

unsigned Inventory::countFreeSlotsNoChecks(uint16_t classIdHash16) const
{
	unsigned count = 0;
	for (const auto& item : items)
	{
		if (item.first == nullptr && item.second < 0)
		{
			count++;
		}
	}
	return count;
}

unsigned Inventory::countFreeSlots(uint16_t classIdHash16) const
{
	unsigned count = 0;
	if (isTypeAllowed(classIdHash16) == true)
	{
		count = countFreeSlotsNoChecks(classIdHash16);
	}
	return count;
}

LevelObjValue Inventory::addQuantity(const ItemClass& itemClass,
	LevelObjValue amount, InventoryPosition invPos, Queryable* itemOwner)
{
	if (amount == 0 ||
		itemClass.getDefaultByHash(ItemProp::Capacity) <= 0 ||
		isTypeAllowed(itemClass.TypeHash16()) == false)
	{
		return amount;
	}
	bool remove = amount < 0;
	size_t itemIdx = 0;
	Item* item;

	if (remove == true)
	{
		// remove from smallest first
		uint32_t freeSlots = 0;
		while ((freeSlots = findFreeQuantity(itemClass.IdHash16(), itemIdx, item)) > 0)
		{
			auto newQuant = item->addQuantity(amount);
			if (newQuant == 0)
			{
				std::unique_ptr<Item> nullItem;
				set(itemIdx, nullItem);
			}
			if (amount == 0)
			{
				return 0;
			}
			itemIdx++;
		}

		// remove from any item of the same type
		itemIdx = 0;
		while (findByClass(itemClass.IdHash16(), itemIdx, item) == true)
		{
			auto newQuant = item->addQuantity(amount);
			if (newQuant == 0)
			{
				std::unique_ptr<Item> nullItem;
				set(itemIdx, nullItem);
			}
			if (amount == 0)
			{
				return 0;
			}
			itemIdx++;
		}
	}
	else
	{
		uint32_t freeSlots = 0;
		while ((freeSlots = findFreeQuantity(itemClass.IdHash16(), itemIdx, item)) > 0)
		{
			item->addQuantity(amount);
			if (amount == 0)
			{
				return 0;
			}
			itemIdx++;
		}
	}
	if (remove == false)
	{
		while (true)
		{
			if (amount <= 0)
			{
				return 0;
			}
			auto newItem = std::make_unique<Item>(&itemClass);
			newItem->updateOwner(itemOwner);
			auto capacity = newItem->getIntByHash(ItemProp::Capacity);
			if (capacity <= 0)
			{
				return amount;
			}
			LevelObjValue quantity = capacity;
			if (amount <= capacity)
			{
				quantity = amount;
			}
			newItem->setIntByHash(ItemProp::Quantity, quantity);

			size_t itemIdx2 = 0;
			if (getFreeSlot(*newItem, itemIdx2, invPos) == true)
			{
				set(itemIdx2, newItem);
				amount -= quantity;
			}
			else
			{
				break;
			}
		}
	}
	return amount;
}

bool Inventory::getQuantity(uint16_t classIdHash16, uint32_t& quantity) const
{
	bool isQuantifiable = false;
	int64_t totalQuantity = 0;
	if (isTypeAllowed(classIdHash16) == true)
	{
		for (const auto& item : (*this))
		{
			LevelObjValue quant;
			if (item.Class()->IdHash16() == classIdHash16)
			{
				if (item.getIntByHash(ItemProp::Quantity, quant) == true)
				{
					totalQuantity += quant;
					isQuantifiable = true;
				}
			}
		}
		if (totalQuantity < 0)
		{
			totalQuantity = 0;
		}
		else if (totalQuantity > std::numeric_limits<LevelObjValue>::max())
		{
			totalQuantity = std::numeric_limits<LevelObjValue>::max();
		}
	}
	quantity = (uint32_t)totalQuantity;
	return isQuantifiable;
}

uint32_t Inventory::getQuantity(uint16_t classIdHash16) const
{
	uint32_t quantity;
	getQuantity(classIdHash16, quantity);
	return quantity;
}

uint32_t Inventory::getMaxCapacity(const ItemClass& itemClass) const
{
	if (itemClass.getDefaultByHash(ItemProp::Capacity) <= 0 ||
		isTypeAllowed(itemClass.IdHash16()) == false)
	{
		return 0;
	}
	uint64_t maxCapacity = 0;
	size_t itemIdx = 0;
	Item* item;
	uint32_t freeQuantity = 0;
	while ((freeQuantity = findFreeQuantity(itemClass.IdHash16(), itemIdx, item)) > 0)
	{
		maxCapacity += freeQuantity;
		itemIdx++;
	}
	auto freeSlots = countFreeSlotsNoChecks(itemClass.IdHash16());
	if (freeSlots > 0)
	{
		auto defaultCapacity = itemClass.getDefaultByHash(ItemProp::Capacity);
		if (defaultCapacity > 0)
		{
			maxCapacity += (uint64_t)defaultCapacity * freeSlots;
			if (maxCapacity >= std::numeric_limits<uint32_t>::max())
			{
				return std::numeric_limits<uint32_t>::max();
			}
		}
	}
	return (uint32_t)maxCapacity;
}

bool Inventory::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	switch (str2int16(props.first))
	{
	case str2int16("capacity"):
		var = Variable((int64_t)items.size());
		break;
	case str2int16("enforceItemSize"):
		var = Variable(enforceItemSize);
		break;
	case str2int16("hasItemClass"):
		var = Variable(hasItem(str2int16(props.second)));
		break;
	case str2int16("isFull"):
		var = Variable(isFull());
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

bool Inventory::updateQuantities(Item* itemToAdd, Item* itemToRemove,
	LevelObjValue& transferedQuantity, bool onlyUpdateIfFullTransfer)
{
	transferedQuantity = 0;
	bool removeItem = false;
	LevelObjValue capacity;
	LevelObjValue capacityOld;
	if (itemToAdd != nullptr &&
		itemToRemove != nullptr &&
		itemToAdd->Class() == itemToRemove->Class() &&
		itemToAdd->getIntByHash(ItemProp::Capacity, capacity) == true &&
		itemToRemove->getIntByHash(ItemProp::Capacity, capacityOld) == true)
	{
		auto quantityA = itemToAdd->getIntByHash(ItemProp::Quantity);
		auto quantityB = itemToRemove->getIntByHash(ItemProp::Quantity);

		if (quantityA + quantityB <= capacity)
		{
			transferedQuantity = quantityB;
			quantityA += quantityB;
			quantityB = 0;
			removeItem = true;
		}
		else if (onlyUpdateIfFullTransfer == true)
		{
			return false;
		}
		else
		{
			transferedQuantity = capacity - quantityA;
			quantityB -= capacity - quantityA;
			quantityA = capacity;
		}
		itemToAdd->setIntByHash(ItemProp::Quantity, quantityA);
		itemToRemove->setIntByHash(ItemProp::Quantity, quantityB);
	}
	return removeItem;
}
