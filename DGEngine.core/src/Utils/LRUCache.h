#pragma once

#include "FixedMap.h"

template <class Key_, class Val_, size_t Size_>
class LRUCache : protected FixedMap<Key_, Val_, Size_>
{
public:
	// if value exists, it gets moved to the top.
	// reference only valid until the next LRUCache call.
	bool getValue(const Key_& key, Val_& value) noexcept
	{
		for (size_t i = 0; i < this->numElements; i++)
		{
			const auto& elem = this->elements[i];
			if (elem.first == key)
			{
				// update LRU
				if (i > 0)
				{
					std::rotate(this->begin(), this->begin() + i, this->begin() + i + 1);
				}
				// return first element
				value = this->elements[0].second;
				return true;
			}
		}
		return false;
	}

	// if value exists, it gets moved to the top.
	// pointer only valid until the next LRUCache call.
	Val_* getValue(const Key_& key) noexcept
	{
		for (size_t i = 0; i < this->numElements; i++)
		{
			const auto& elem = this->elements[i];
			if (elem.first == key)
			{
				// update LRU
				if (i > 0)
				{
					std::rotate(this->begin(), this->begin() + i, this->begin() + i + 1);
				}
				// return first element
				return &this->elements[0].second;
			}
		}
		return nullptr;
	}

	// Updates a value and brings it to the top. Removes last accessed value if full.
	void updateValue(const Key_& key, const Val_& value) noexcept
	{
		bool updated = false;
		size_t i = 0;
		for (; i < this->numElements; i++)
		{
			auto& elem = this->elements[i];
			if (elem.first == key)
			{
				elem.second = value;
				updated = true;
				break;
			}
		}
		if (updated == false)
		{
			// insert element
			if (i < this->elements.size())
			{
				this->elements[i] = std::make_pair(key, value);
				this->numElements++;
			}
			// override last element
			else
			{
				i--;
				this->elements[i] = std::make_pair(key, value);
			}
		}
		// update LRU
		if (i > 0)
		{
			std::rotate(this->begin(), this->begin() + i, this->begin() + i + 1);
		}
	}
};
