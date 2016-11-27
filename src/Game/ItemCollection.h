#pragma once

#include "Item.h"
#include <memory>
#include <vector>

class ItemCollection
{
private:
	std::vector<std::shared_ptr<Item>> items;
	sf::Vector2u size;
	std::vector<unsigned> allowedTypes;

public:
	ItemCollection() {}
	ItemCollection(size_t size_);
	ItemCollection(const sf::Vector2u& size_);

	void init(size_t size_);
	void init(const sf::Vector2u& size_);

	const std::vector<std::shared_ptr<Item>>& Items() const { return items; }

	size_t Size() const { return items.size(); }

	void allowType(const std::string& type);

	bool isTypeAllowed(const std::string& type) const;
	bool isTypeAllowed(unsigned typeHash) const;

	std::shared_ptr<Item>& operator[] (size_t idx) { return items[idx]; }
	const std::shared_ptr<Item>& operator[] (size_t idx) const { return items[idx]; }

	const std::shared_ptr<Item>& get(size_t idx) const { return items[idx]; }
	const std::shared_ptr<Item>& get(size_t x, size_t y) const { return items[x + y * size.y]; }
	const std::shared_ptr<Item>& get(const sf::Vector2u& pos) const { return get(pos.x, pos.y); }

	void set(size_t idx, const std::shared_ptr<Item>& item);
	void set(const sf::Vector2u& position, const std::shared_ptr<Item>& item);
};
