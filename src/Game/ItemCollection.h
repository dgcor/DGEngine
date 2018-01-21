#pragma once

#include "GameProperties.h"
#include "Item.h"
#include "ItemXY.h"
#include <iterator>
#include <memory>
#include "Utils/iterator_tpl.h"
#include <vector>

class ItemCollection
{
private:
	typedef std::pair<std::unique_ptr<Item>, int32_t> ItemCell;

	std::vector<ItemCell> items;
	ItemXY size;
	std::vector<uint16_t> allowedTypes;
	bool enforceItemSize{ false };

	bool setAndDontEnforceItemSize(size_t idx,
		std::unique_ptr<Item>& item, std::unique_ptr<Item>& oldItem);

	bool setAndEnforceItemSize(const ItemXY& position,
		std::unique_ptr<Item>& item, std::unique_ptr<Item>& oldItem);

	bool isItemSlotEmpty(int x, int y,
		const ItemXY& itemSize, size_t& itemIdx) const;

	void resetIndexes() noexcept;

public:
	struct it_state
	{
		size_t idx;
		inline void next(const ItemCollection* ref)
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
		inline void prev(const ItemCollection* ref)
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
		inline void begin(const ItemCollection* ref)
		{
			idx = 0;
			if (ref->items[idx].first == nullptr)
			{
				next(ref);
			}
		}
		inline void end(const ItemCollection* ref) noexcept
		{
			idx = ref->items.size();
		}
		inline Item& get(ItemCollection* ref)
		{
			return *ref->items[idx].first;
		}
		inline const Item& get(const ItemCollection* ref)
		{
			return *ref->items[idx].first;
		}
		inline bool cmp(const it_state& s) const noexcept
		{
			return idx != s.idx;
		}
	};

	SETUP_ITERATORS(ItemCollection, const Item&, it_state);
	SETUP_REVERSE_ITERATORS(ItemCollection, const Item&, it_state);

	ItemCollection() noexcept {}
	ItemCollection(size_t size_);
	ItemCollection(const ItemXY& size_);

	void init(size_t size_);
	void init(const ItemXY& size_);

	size_t Size() const noexcept { return items.size(); }

	void setEnforceItemSize(bool enforceItemSize_) noexcept { enforceItemSize = enforceItemSize_; }

	void allowType(const std::string& type);

	bool isTypeAllowed(const std::string& type) const;
	bool isTypeAllowed(uint16_t typeHash16) const;

	size_t getIndex(size_t x, size_t y) const noexcept { return x + y * size.x; }
	size_t getIndex(const ItemXY& pos) const noexcept { return getIndex(pos.x, pos.y); }

	Item* get(size_t idx) const;
	Item* get(size_t x, size_t y) const { return get(x + y * size.x); }
	Item* get(const ItemXY& pos) const { return get(pos.x, pos.y); }

	bool set(size_t idx, std::unique_ptr<Item>& item);
	bool set(size_t idx, std::unique_ptr<Item>& item, std::unique_ptr<Item>& oldItem);

	bool set(const ItemXY& position, std::unique_ptr<Item>& item);
	bool set(const ItemXY& position, std::unique_ptr<Item>& item,
		std::unique_ptr<Item>& oldItem);

	bool isFull() const noexcept;

	bool isItemSlotInUse(size_t idx) const;
	bool isItemSlotInUse(const ItemXY& position) const;

	bool getFreeItemSlot(const Item& item, size_t& itemIdx,
		InventoryPosition invPos = InventoryPosition::TopLeft) const;

	bool hasFreeItemSlot(const Item& item) const;

	bool find(uint16_t itemTypeHash16,
		size_t& idx, Item*& item) const;

	unsigned countFreeSlots(const ItemClass& itemClass) const;

	bool getProperty(const std::string& prop, Variable& var) const;
};
