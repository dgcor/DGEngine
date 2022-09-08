#pragma once

#include <cstdint>
#include "Game/Item/Item.h"
#include "Game/LevelObject/LevelObject.h"
#include "LevelFlags.h"
#include <vector>

class LevelCell
{
public:
	// number of layers including sol layer
	static constexpr uint32_t NumberOfLayers = 12;
	static constexpr uint32_t FlagsLayer = NumberOfLayers - 1;

private:
	std::array<int32_t, NumberOfLayers> tileIndexes{ -1, -1, -1, -1,- 1, -1, -1, -1, -1, -1, -1, 0 };
	std::vector<LevelObject*> objects;

public:
	auto begin() noexcept { return objects.begin(); }
	auto end() noexcept { return objects.end(); }
	auto begin() const noexcept { return objects.begin(); }
	auto end() const noexcept { return objects.end(); }
	auto cbegin() const noexcept { return objects.cbegin(); }
	auto cend() const noexcept { return objects.cend(); }
	auto rbegin() noexcept { return objects.rbegin(); }
	auto rend() noexcept { return objects.rend(); }
	auto rbegin() const noexcept { return objects.rbegin(); }
	auto rend() const noexcept { return objects.rend(); }
	auto crbegin() const noexcept { return objects.crbegin(); }
	auto crend() const noexcept { return objects.crend(); }

	LevelCell() {}
	LevelCell(int32_t defaultTileLayer1)
		: tileIndexes{ defaultTileLayer1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0 } {}

	int32_t getTileIndex(size_t layer) const noexcept { return tileIndexes[layer]; }

	void setTileIndex(size_t layer, int32_t tileIndex_) noexcept { tileIndexes[layer] = tileIndex_; }

	bool PassableIgnoreObject() const noexcept { return LevelFlags::Passable(tileIndexes[FlagsLayer]); }
	bool PassableIgnoreObject(const LevelObject* ignoreObj) const;
	bool Passable() const;

	LevelObject* back() const;
	LevelObject* front() const;

	bool hasObjects() const noexcept { return objects.empty() == false; }

	template <class T>
	T* getObject() const noexcept
	{
		for (const auto object : objects)
		{
			const auto castObj = dynamic_cast<T*>(object);
			if (castObj != nullptr)
			{
				return castObj;
			}
		}
		return nullptr;
	}

	void addFront(LevelObject* obj);
	void addBack(LevelObject* obj);
	bool removeObject(const LevelObject* obj);

	template <class T>
	T* removeObject()
	{
		for (auto it = objects.begin(); it != objects.end(); ++it)
		{
			auto oldObj = dynamic_cast<T*>(*it);
			if (oldObj != nullptr)
			{
				objects.erase(it);
				return oldObj;
			}
		}
		return nullptr;
	}
};
