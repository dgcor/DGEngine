#pragma once

#include "GameProperties.h"
#include "Item.h"
#include <memory>
#include <vector>

class ItemCollection
{
private:
	std::vector<std::shared_ptr<Item>> items;
	sf::Vector2u size;
	std::vector<unsigned> allowedTypes;
	bool enforceItemSize{ false };

	bool setAndDontEnforceItemSize(size_t idx,
		const std::shared_ptr<Item>& item, std::shared_ptr<Item>& oldItem);

	bool setAndEnforceItemSize(const sf::Vector2u& position,
		const std::shared_ptr<Item>& item, std::shared_ptr<Item>& oldItem);

	bool isItemSlotEmpty(int x, int y,
		const sf::Vector2u& itemSize, size_t& itemIdx) const;

public:
	ItemCollection() {}
	ItemCollection(size_t size_);
	ItemCollection(const sf::Vector2u& size_);

	void init(size_t size_);
	void init(const sf::Vector2u& size_);

	const std::vector<std::shared_ptr<Item>>& Items() const { return items; }

	size_t Size() const { return items.size(); }

	void setEnforceItemSize(bool enforceItemSize_) { enforceItemSize = enforceItemSize_; }

	void allowType(const std::string& type);

	bool isTypeAllowed(const std::string& type) const;
	bool isTypeAllowed(unsigned typeHash) const;

	size_t getIndex(size_t x, size_t y) const { return x + y * size.x; }
	size_t getIndex(const sf::Vector2u& pos) const { return getIndex(pos.x, pos.y); }

	const std::shared_ptr<Item>& operator[] (size_t idx) const { return items[idx]; }

	const std::shared_ptr<Item>& get(size_t idx) const { return items[idx]; }
	const std::shared_ptr<Item>& get(size_t x, size_t y) const { return items[x + y * size.x]; }
	const std::shared_ptr<Item>& get(const sf::Vector2u& pos) const { return get(pos.x, pos.y); }

	bool set(size_t idx, const std::shared_ptr<Item>& item);
	bool set(size_t idx, const std::shared_ptr<Item>& item,
		std::shared_ptr<Item>& oldItem);

	bool set(const sf::Vector2u& position, const std::shared_ptr<Item>& item);
	bool set(const sf::Vector2u& position, const std::shared_ptr<Item>& item,
		std::shared_ptr<Item>& oldItem);

	bool isFull() const;

	bool isItemSlotInUse(size_t idx) const;
	bool isItemSlotInUse(const sf::Vector2u& position) const;

	bool getItemSlot(const sf::Vector2u& itemSize,
		size_t& itemIdx, InventoryPosition invPos = InventoryPosition::TopLeft) const;

	bool getProperty(const std::string& prop, Variable& var) const;
};
