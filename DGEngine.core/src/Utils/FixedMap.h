#pragma once

#include <algorithm>
#include <array>
#include <utility>

template <class Key_, class Val_, size_t Size_>
class FixedMap
{
protected:
	typedef std::array<std::pair<Key_, Val_>, Size_> Elements;

	size_t numElements{ 0 };
	Elements elements;

public:
	using iterator = typename Elements::iterator;
	using const_iterator = typename Elements::const_iterator;
	using reverse_iterator = typename Elements::reverse_iterator;
	using const_reverse_iterator = typename Elements::const_reverse_iterator;

	constexpr iterator begin() noexcept { return elements.begin(); }
	constexpr iterator end() noexcept { return elements.begin() + numElements; }
	constexpr const_iterator begin() const noexcept { return elements.begin(); }
	constexpr const_iterator end() const noexcept { return elements.begin() + numElements; }
	constexpr const_iterator cbegin() const noexcept { return elements.cbegin(); }
	constexpr const_iterator cend() const noexcept { return elements.cbegin() + numElements; }
	constexpr reverse_iterator rbegin() noexcept { return elements.rend() - numElements; }
	constexpr reverse_iterator rend() noexcept { return elements.rend(); }
	constexpr const_reverse_iterator rbegin() const noexcept { return elements.rend() - numElements; }
	constexpr const_reverse_iterator rend() const noexcept { return elements.rend(); }
	constexpr const_reverse_iterator crbegin() const noexcept { return elements.crend() - numElements; }
	constexpr const_reverse_iterator crend() const noexcept { return elements.crend(); }

	constexpr size_t empty() const noexcept { return numElements == 0; }

	constexpr size_t size() const noexcept { return numElements; }

	constexpr size_t max_size() const noexcept { return elements.size(); }

	bool hasValue(const Key_& key) const noexcept
	{
		if (numElements > 0)
		{
			for (size_t i = 0; i < numElements; i++)
			{
				if (elements[i].first == key)
				{
					return true;
				}
			}
		}
		return false;
	}

	bool getValue(const Key_& key, Val_& value) const noexcept
	{
		for (size_t i = 0; i < numElements; i++)
		{
			const auto& elem = elements[i];
			if (elem.first == key)
			{
				value = elem.second;
				return true;
			}
		}
		return false;
	}

	const Val_* getValue(const Key_& key) const noexcept
	{
		for (size_t i = 0; i < numElements; i++)
		{
			const auto& elem = elements[i];
			if (elem.first == key)
			{
				return &elem.second;
			}
		}
		return nullptr;
	}

	Val_* getValue(const Key_& key) noexcept
	{
		return const_cast<Val_*>(static_cast<const FixedMap*>(this)->getValue(key));
	}

	bool setValue(const Key_& key, const Val_& value)
	{
		size_t i = 0;
		for (; i < numElements; i++)
		{
			auto& elem = elements[i];
			if (elem.first == key)
			{
				elem.second = value;
				return true;
			}
		}
		if (i < elements.size())
		{
			elements[i] = std::make_pair(key, value);
			numElements++;
			return true;
		}
		return false;
	}

	void deleteValue(const Key_& key)
	{
		for (size_t i = 0; i < numElements; i++)
		{
			if (elements[i].first == key)
			{
				elements[i].second = {};
				std::move(elements.begin() + i + 1,
					elements.begin() + numElements,
					elements.begin() + i);
				numElements--;
				return;
			}
		}
	}
};
