#pragma once

#include <memory>
#include <variant>

class Queryable;

using QueryObject = std::variant<
	const Queryable*,
	std::shared_ptr<Queryable>,
	std::pair<const Queryable*, size_t>,
	std::pair<std::shared_ptr<Queryable>, size_t>>;

template <class... T>
constexpr bool holdsQueryablePtr(const std::variant<T...>& v) noexcept { return std::holds_alternative<const Queryable*>(v); }

template <class... T>
constexpr bool holdsQueryableSharedPtr(const std::variant<T...>& v) noexcept { return std::holds_alternative<std::shared_ptr<Queryable>>(v); }

template <class... T>
constexpr bool holdsQueryablePtrIndex(const std::variant<T...>& v) noexcept { return std::holds_alternative<std::pair<const Queryable*, size_t>>(v); }

template <class... T>
constexpr bool holdsQueryableSharedPtrIndex(const std::variant<T...>& v) noexcept { return std::holds_alternative<std::pair<std::shared_ptr<Queryable>, size_t>>(v); }

template <class... T>
constexpr const Queryable* getQueryablePtr(const std::variant<T...>& v) noexcept
{
	if (holdsQueryablePtr(v) == true)
	{
		return std::get<const Queryable*>(v);
	}
	else if (holdsQueryableSharedPtr(v) == true)
	{
		return std::get<std::shared_ptr<Queryable>>(v).get();
	}
	else if (holdsQueryablePtrIndex(v) == true)
	{
		return std::get<std::pair<const Queryable*, size_t>>(v).first;
	}
	else if (holdsQueryableSharedPtrIndex(v) == true)
	{
		return std::get<std::pair<std::shared_ptr<Queryable>, size_t>>(v).first.get();
	}
	return nullptr;
}

template <class... T>
constexpr size_t getQueryablePtrIndex(const std::variant<T...>& v, size_t defaultIndex) noexcept
{
	if (holdsQueryablePtrIndex(v) == true)
	{
		return std::get<std::pair<const Queryable*, size_t>>(v).second;
	}
	else if (holdsQueryableSharedPtrIndex(v) == true)
	{
		return std::get<std::pair<std::shared_ptr<Queryable>, size_t>>(v).second;
	}
	return defaultIndex;
}
