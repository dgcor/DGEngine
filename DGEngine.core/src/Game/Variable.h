#pragma once

#include <cstdint>
#include <string>
#include <tuple>
#include <variant>

using Int64Tuple2 = std::tuple<int64_t, int64_t>;
using Int64Tuple3 = std::tuple<int64_t, int64_t, int64_t>;
using Int64Tuple4 = std::tuple<int64_t, int64_t, int64_t, int64_t>;

using DoubleTuple2 = std::tuple<double, double>;
using DoubleTuple3 = std::tuple<double, double, double>;
using DoubleTuple4 = std::tuple<double, double, double, double>;

using Variable = std::variant<
	std::string,
	int64_t,
	double,
	bool,
	Int64Tuple2,
	Int64Tuple3,
	Int64Tuple4,
	DoubleTuple2,
	DoubleTuple3,
	DoubleTuple4>;

template <class... T>
constexpr bool holdsVariable(const std::variant<T...>& v) noexcept { return std::holds_alternative<Variable>(v); }
