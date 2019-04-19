#pragma once

#include "GameProperties.h"
#include "Item.h"
#include <iterator>
#include <memory>
#include "PairXY.h"
#include "Utils/iterator_tpl.h"
#include <vector>

class Inventory
{
private:
	// if index >= 0, it points to the item.
	typedef std::pair<std::unique_ptr<Item>, int32_t> ItemCell;

	std::vector<ItemCell> items;
	PairUInt8 size;
	std::vector<uint16_t> allowedTypes;
	bool enforceItemSize{ false };

	// Doesn't perform the check for allowed class types. only for items whose size is 1
	unsigned countFreeSlotsNoChecks(uint16_t classIdHash16) const;

	bool setAndDontEnforceItemSize(size_t idx, std::unique_ptr<Item>& item,
		std::unique_ptr<Item>& oldItem);

	bool setAndEnforceItemSize(const PairUInt8& position, std::unique_ptr<Item>& item,
		std::unique_ptr<Item>& oldItem);

	bool isSlotEmpty(int x, int y,
		const PairUInt8& itemSize, size_t& itemIdx) const;

	void resetIndexes() noexcept;

public:
	struct it_state
	{
		size_t idx;
		inline void next(const Inventory* ref)
		{
			idx++;
			for (; idx < ref->items.size(); idx++)
			{
				if (ref->items[idx].first == nullptr)
				{
					continue;
				}
				return;
			}
		}
		inline void prev(const Inventory* ref)
		{
			auto size = ref->items.size();
			if (size > 0)
			{
				idx--;
				for (; idx < size; idx--)
				{
					if (ref->items[idx].first == nullptr)
					{
						continue;
					}
					return;
				}
			}
			idx = size;
		}
		inline void begin(const Inventory* ref)
		{
			idx = 0;
			if (ref->items.empty() == false &&
				ref->items[idx].first == nullptr)
			{
				next(ref);
			}
		}
		inline void end(const Inventory* ref) noexcept
		{
			idx = ref->items.size();
		}
		inline Item& get(Inventory* ref)
		{
			return *ref->items[idx].first;
		}
		inline const Item& get(const Inventory* ref)
		{
			return *ref->items[idx].first;
		}
		inline bool cmp(const it_state& s) const noexcept
		{
			return idx != s.idx;
		}
	};

	SETUP_ITERATORS(Inventory, const Item&, it_state);
	SETUP_REVERSE_ITERATORS(Inventory, const Item&, it_state);

	Inventory() noexcept {}
	Inventory(size_t size_);
	Inventory(const PairUInt8& size_);

	void init(size_t size_);
	void init(const PairUInt8& size_);

	bool empty() const noexcept;

	size_t Size() const noexcept { return items.size(); }
	const PairUInt8& getXYSize() const noexcept { return size; }

	bool getEnforceItemSize() const noexcept { return enforceItemSize; }
	void setEnforceItemSize(bool enforceItemSize_) noexcept { enforceItemSize = enforceItemSize_; }

	const std::vector<uint16_t>& AllowedTypes() const noexcept { return allowedTypes; }

	void allowType(const std::string_view type);
	void allowTypeByHash(uint16_t typeHash16);

	bool isTypeAllowed(const std::string_view type) const;
	bool isTypeAllowed(uint16_t typeHash16) const;

	size_t getIndex(size_t x, size_t y) const noexcept { return x + y * size.x; }
	size_t getIndex(const PairUInt8& pos) const noexcept { return getIndex(pos.x, pos.y); }

	Item* get(size_t idx) const;
	Item* get(size_t x, size_t y) const { return get(x + y * size.x); }
	Item* get(const PairUInt8& pos) const { return get(pos.x, pos.y); }

	bool set(size_t idx, std::unique_ptr<Item>& item);
	bool set(size_t idx, std::unique_ptr<Item>& item, std::unique_ptr<Item>& oldItem);

	bool set(const PairUInt8& position, std::unique_ptr<Item>& item);
	bool set(const PairUInt8& position, std::unique_ptr<Item>& item,
		std::unique_ptr<Item>& oldItem);

	bool isFull() const noexcept;

	// only checks the specified slot. if a slot is indexing another slot, it returns false.
	bool isSlotInUse(size_t idx) const;
	// only checks the specified slot. if a slot is indexing another slot, it returns false.
	bool isSlotInUse(const PairUInt8& position) const;

	bool getFreeSlot(const Item& item,
		size_t& itemIdx, InventoryPosition invPos) const;

	bool hasFreeSlot(const Item& item) const;

	bool hasItem(uint16_t classIdHash16) const;

	// finds an item by the item's class id
	// returns true if successful and sets both index and item
	bool findByClass(uint16_t classIdHash16, size_t& idx, Item*& item) const;

	// finds an item with capacity/quantity that has quantity < capacity
	// returns the free quantity or 0 if no items with free quantity exists
	uint32_t findFreeQuantity(uint16_t classIdHash16, size_t& idx, Item*& itemFound) const;

	// finds an item with capacity/quantity that has the biggest free quantity
	// that can store the quantity needed (by default 0).
	// returns the free quantity or 0 if no items with free quantity exists
	uint32_t findBiggestFreeQuantity(uint16_t classIdHash16,
		Item*& itemFound, uint32_t quantityNeded = 0) const;

	// finds an item with capacity/quantity that has the smallest free quantity
	// that can store the quantity needed (by default 0).
	// returns the free quantity or 0 if no items with free quantity exists
	uint32_t findSmallestFreeQuantity(uint16_t classIdHash16,
		Item*& itemFound, uint32_t quantityNeded = 0) const;

	// only for items whose size is 1
	unsigned countFreeSlots(uint16_t classIdHash16) const;

	// returns the remaining quantity to add/remove. 0 if all quantity was added.
	LevelObjValue addQuantity(const ItemClass& itemClass,
		LevelObjValue amount, InventoryPosition invPos, Queryable* itemOwner);

	// Gets the sum of all the quantities for a specified item class id or 0.
	// returns true if the item is quantifiable or false (and sets quantity to 0) otherwise.
	bool getQuantity(uint16_t classIdHash16, uint32_t& quantity) const;

	// returns the sum of all the quantities for a specified item class id.
	uint32_t getQuantity(uint16_t classIdHash16) const;

	uint32_t getMaxCapacity(const ItemClass& itemClass) const;

	bool getProperty(const std::string_view prop, Variable& var) const;

	// Transfers the quantity in the second item to the first item.
	// onlyUpdateIfFullTransfer - if true, only updates the first item if the second item's quantity fits in the first
	// return true if the second item's quantity is 0.
	static bool updateQuantities(Item* itemToAdd, Item* itemToRemove,
		LevelObjValue& transferedQuantity, bool onlyUpdateIfFullTransfer = false);
};
