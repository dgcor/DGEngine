#pragma once

#include "GameProperties.h"
#include "Item.h"
#include "ItemXY.h"
#include <memory>
#include <vector>

class ItemCollection
{
private:
	std::vector<std::shared_ptr<Item>> items;
	ItemXY size;
	std::vector<uint32_t> allowedTypes;
	bool enforceItemSize{ false };

	bool setAndDontEnforceItemSize(size_t idx,
		const std::shared_ptr<Item>& item, std::shared_ptr<Item>& oldItem);

	bool setAndEnforceItemSize(const ItemXY& position,
		const std::shared_ptr<Item>& item, std::shared_ptr<Item>& oldItem);

	bool isItemSlotEmpty(int x, int y,
		const ItemXY& itemSize, size_t& itemIdx) const;

public:
	using iterator = std::vector<std::shared_ptr<Item>>::iterator;
	using const_iterator = std::vector<std::shared_ptr<Item>>::const_iterator;
	using reverse_iterator = std::vector<std::shared_ptr<Item>>::reverse_iterator;
	using const_reverse_iterator = std::vector<std::shared_ptr<Item>>::const_reverse_iterator;

	iterator begin() { return items.begin(); }
	iterator end() { return items.end(); }
	const_iterator begin() const { return items.begin(); }
	const_iterator end() const { return items.end(); }
	const_iterator cbegin() const { return items.cbegin(); }
	const_iterator cend() const { return items.cend(); }
	reverse_iterator rbegin() { return items.rbegin(); }
	reverse_iterator rend() { return items.rend(); }
	const_reverse_iterator rbegin() const { return items.rbegin(); }
	const_reverse_iterator rend() const { return items.rend(); }
	const_reverse_iterator crbegin() const { return items.crbegin(); }
	const_reverse_iterator crend() const { return items.crend(); }

	ItemCollection() {}
	ItemCollection(size_t size_);
	ItemCollection(const ItemXY& size_);

	void init(size_t size_);
	void init(const ItemXY& size_);

	const std::vector<std::shared_ptr<Item>>& Items() const { return items; }

	size_t Size() const { return items.size(); }

	void setEnforceItemSize(bool enforceItemSize_) { enforceItemSize = enforceItemSize_; }

	void allowType(const std::string& type);

	bool isTypeAllowed(const std::string& type) const;
	bool isTypeAllowed(uint32_t typeHash) const;

	size_t getIndex(size_t x, size_t y) const { return x + y * size.x; }
	size_t getIndex(const ItemXY& pos) const { return getIndex(pos.x, pos.y); }

	const std::shared_ptr<Item>& operator[] (size_t idx) const { return items[idx]; }

	const std::shared_ptr<Item>& get(size_t idx) const { return items[idx]; }
	const std::shared_ptr<Item>& get(size_t x, size_t y) const { return items[x + y * size.x]; }
	const std::shared_ptr<Item>& get(const ItemXY& pos) const { return get(pos.x, pos.y); }

	bool set(size_t idx, const std::shared_ptr<Item>& item);
	bool set(size_t idx, const std::shared_ptr<Item>& item,
		std::shared_ptr<Item>& oldItem);

	bool set(const ItemXY& position, const std::shared_ptr<Item>& item);
	bool set(const ItemXY& position, const std::shared_ptr<Item>& item,
		std::shared_ptr<Item>& oldItem);

	bool isFull() const;

	bool isItemSlotInUse(size_t idx) const;
	bool isItemSlotInUse(const ItemXY& position) const;

	bool getItemSlot(const ItemXY& itemSize, size_t& itemIdx,
		InventoryPosition invPos = InventoryPosition::TopLeft) const;

	bool getProperty(const std::string& prop, Variable& var) const;
};
