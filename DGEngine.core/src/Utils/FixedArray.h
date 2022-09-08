#pragma once

#include <array>

template <class Val_, size_t Size_>
class FixedArray
{
protected:
	using Elements = std::array<Val_, Size_>;

	size_t numElements{ 0 };
	Elements elements;

public:
	constexpr auto begin() noexcept { return elements.begin(); }
	constexpr auto end() noexcept { return elements.begin() + numElements; }
	constexpr auto begin() const noexcept { return elements.begin(); }
	constexpr auto end() const noexcept { return elements.begin() + numElements; }
	constexpr auto cbegin() const noexcept { return elements.cbegin(); }
	constexpr auto cend() const noexcept { return elements.cbegin() + numElements; }
	constexpr auto rbegin() noexcept { return elements.rend() - numElements; }
	constexpr auto rend() noexcept { return elements.rend(); }
	constexpr auto rbegin() const noexcept { return elements.rend() - numElements; }
	constexpr auto rend() const noexcept { return elements.rend(); }
	constexpr auto crbegin() const noexcept { return elements.crend() - numElements; }
	constexpr auto crend() const noexcept { return elements.crend(); }

	void clear()
	{
		numElements = 0;
		elements.fill({});
	}

	constexpr Val_* data() noexcept { return elements.data(); }
	constexpr const Val_* data() const noexcept { return elements.data(); }

	constexpr bool empty() const noexcept { return numElements == 0; }

	constexpr bool full() const noexcept { return numElements >= elements.size(); }

	constexpr auto size() const noexcept { return numElements; }

	constexpr auto max_size() const noexcept { return elements.size(); }

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
