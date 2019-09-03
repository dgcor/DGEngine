#pragma once

#include <array>

template <class Val_, size_t Size_>
class FixedArray
{
protected:
	typedef std::array<Val_, Size_> Elements;

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

	void clear()
	{
		numElements = 0;
		elements.fill({});
	}

	constexpr Val_* data() noexcept { return elements.data(); }
	constexpr const Val_* data() const noexcept { return elements.data(); }

	constexpr bool empty() const noexcept { return numElements == 0; }

	constexpr bool full() const noexcept { return numElements >= elements.size(); }

	constexpr size_t size() const noexcept { return numElements; }

	constexpr size_t max_size() const noexcept { return elements.size(); }

	constexpr bool push_back(const Val_& val)
	{
		if (numElements < elements.size())
		{
			elements[numElements] = val;
			numElements++;
			return true;
		}
		return false;
	}

	constexpr Val_& operator[](const size_t idx) noexcept { return elements[idx]; }
	constexpr const Val_& operator[](const size_t idx) const noexcept { return elements[idx]; }
};
