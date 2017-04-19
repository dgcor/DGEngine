#pragma once

#include <cstdint>
#include "LevelObject.h"
#include "Item.h"
#include <vector>

class LevelCell
{
private:
	int16_t minIndex{ -1 };
	int8_t sol{ 0 };
	std::vector<std::shared_ptr<LevelObject>> objects;

public:
	using iterator = std::vector<std::shared_ptr<LevelObject>>::iterator;
	using const_iterator = std::vector<std::shared_ptr<LevelObject>>::const_iterator;
	using reverse_iterator = std::vector<std::shared_ptr<LevelObject>>::reverse_iterator;
	using const_reverse_iterator = std::vector<std::shared_ptr<LevelObject>>::const_reverse_iterator;

	iterator begin() { return objects.begin(); }
	iterator end() { return objects.end(); }
	const_iterator begin() const { return objects.begin(); }
	const_iterator end() const { return objects.end(); }
	const_iterator cbegin() const { return objects.cbegin(); }
	const_iterator cend() const { return objects.cend(); }
	reverse_iterator rbegin() { return objects.rbegin(); }
	reverse_iterator rend() { return objects.rend(); }
	const_reverse_iterator rbegin() const { return objects.rbegin(); }
	const_reverse_iterator rend() const { return objects.rend(); }
	const_reverse_iterator crbegin() const { return objects.crbegin(); }
	const_reverse_iterator crend() const { return objects.crend(); }

	int16_t MinIndex() const { return minIndex; }
	void MinIndex(int16_t minIndex_) { minIndex = minIndex_; }
	void Sol(int8_t sol_) { sol = sol_; }

	bool PassableIgnoreObject() const { return !(sol & 0x01); }
	bool Passable() const;

	std::shared_ptr<LevelObject> back() const;
	std::shared_ptr<LevelObject> front() const;

	std::shared_ptr<LevelObject> getObject(LevelObject* obj) const;

	template <class T>
	std::shared_ptr<T> getObject() const
	{
		for (const auto& object : objects)
		{
			auto castObj = std::dynamic_pointer_cast<T>(object);
			if (castObj != nullptr)
			{
				return castObj;
			}
		}
		return nullptr;
	}

	void addFront(const std::shared_ptr<LevelObject>& obj);
	void addBack(const std::shared_ptr<LevelObject>& obj);
	void deleteObject(LevelObject* obj);
};
