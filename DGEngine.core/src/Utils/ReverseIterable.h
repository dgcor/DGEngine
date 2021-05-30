#pragma once

#include <iterator>

template <typename T>
struct reversion_wrapper { T& iterable; };

template <typename T>
auto begin(reversion_wrapper<T> w) noexcept { return std::rbegin(w.iterable); }

template <typename T>
auto end(reversion_wrapper<T> w) noexcept { return std::rend(w.iterable); }

template <typename T>
reversion_wrapper<T> reverse(T&& iterable) noexcept { return{ iterable }; }
